//
// Network interface
//
#include <net/net.h>

struct netif *netif_list = NULL;
struct netif *netif_default = NULL;

static int netif_proc(struct proc_file *pf, void *arg) {
    struct netif *netif;

    for (netif = netif_list; netif != NULL; netif = netif->next) {
        pprintf(pf, "%s: addr %a mask %a gw %a\n", netif->name, &netif->ipaddr, &netif->netmask, &netif->gw);
    }

    return 0;
}

void netif_init() {
    register_proc_inode("netif", netif_proc, NULL);
}

struct netif *netif_add(char *name, struct ip_addr *ipaddr, struct ip_addr *netmask, struct ip_addr *gw) {
    struct netif *netif;

    netif = kmalloc(sizeof(struct netif));
    if (!netif) return NULL;
    memset(netif, 0, sizeof(struct netif));

    strcpy(netif->name, name);
    netif->input = ip_input;
    netif->output = NULL;
    netif->flags = NETIF_ALLMULTI;

    ip_addr_set(&netif->ipaddr, ipaddr);
    ip_addr_set(&netif->netmask, netmask);
    ip_addr_set(&netif->gw, gw);
    netif->broadcast.addr = (ipaddr->addr & netmask->addr) | ~netmask->addr;

    netif->mclist = NULL;
    netif->mccount = 0;

    netif->next = netif_list;
    netif_list = netif;

    return netif;
}

struct netif *netif_find(char *name) {
    struct netif *netif;

    if (!name) return NULL;

    for (netif = netif_list; netif != NULL; netif = netif->next) {
        if (strcmp(name, netif->name) == 0) return netif;
    }

    return NULL;
}

void netif_set_ipaddr(struct netif *netif, struct ip_addr *ipaddr) {
    ip_addr_set(&netif->ipaddr, ipaddr);
}

void netif_set_gw(struct netif *netif, struct ip_addr *gw) {
    ip_addr_set(&netif->gw, gw);
}

void netif_set_netmask(struct netif *netif, struct ip_addr *netmask) {
    ip_addr_set(&netif->netmask, netmask);
}

void netif_set_default(struct netif *netif) {
    netif_default = netif;
}

int netif_ioctl_list(void *data, size_t size) {
    int numifs;
    struct netif *netif;
    struct ifcfg *ifcfg;
    struct sockaddr_in *sin;

    if (!data) return -EFAULT;

    // Find number of network interfaces
    numifs = 0;
    netif = netif_list;
    while (netif) {
        numifs++;
        netif = netif->next;
    }

    // Fill interface info into buffer
    if (size >= (size_t)(numifs * sizeof(struct ifcfg))) {
        netif = netif_list;
        ifcfg = (struct ifcfg *) data;
        while (netif) {
            memset(ifcfg, 0, sizeof(struct ifcfg));

            strcpy(ifcfg->name, netif->name);

            sin = (struct sockaddr_in *) &ifcfg->addr;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = netif->ipaddr.addr;

            sin = (struct sockaddr_in *) &ifcfg->gw;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = netif->gw.addr;

            sin = (struct sockaddr_in *) &ifcfg->netmask;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = netif->netmask.addr;

            sin = (struct sockaddr_in *) &ifcfg->broadcast;
            sin->sin_family = AF_INET;
            sin->sin_addr.s_addr = netif->broadcast.addr;

            memcpy(ifcfg->hwaddr, &netif->hwaddr, sizeof(struct eth_addr));

            if (netif->flags & NETIF_UP) ifcfg->flags |= IFCFG_UP;
            if (netif->flags & NETIF_DHCP) ifcfg->flags |= IFCFG_DHCP;
            if (netif->flags & NETIF_LOOPBACK) ifcfg->flags |= IFCFG_LOOPBACK;
            if (netif == netif_default) ifcfg->flags |= IFCFG_DEFAULT;

            netif = netif->next;
            ifcfg++;
        }
    }

    return numifs * sizeof(struct ifcfg);
}

int netif_ioctl_cfg(void *data, size_t size) {
    struct netif *netif;
    struct ifcfg *ifcfg;
    struct dhcp_state *state;

    if (!data) return -EFAULT;
    if (size != sizeof(struct ifcfg)) return -EINVAL;
    ifcfg = (struct ifcfg *) data;

    netif = netif_find(ifcfg->name);
    if (!netif) return -ENXIO;

    // Check for interface down
    if ((ifcfg->flags & IFCFG_UP) == 0 && (netif->flags & NETIF_UP) == 1) {
        // Release DHCP lease
        dhcp_stop(netif);
        netif->flags &= ~NETIF_UP;
    }

    // Update network interface configuration
    if (ifcfg->flags & IFCFG_DHCP) {
        netif->flags |= NETIF_DHCP;
    } else {
        netif->flags &= ~NETIF_DHCP;
    }

    netif->ipaddr.addr = ((struct sockaddr_in *) &ifcfg->addr)->sin_addr.s_addr;
    netif->netmask.addr = ((struct sockaddr_in *) &ifcfg->netmask)->sin_addr.s_addr;
    netif->gw.addr = ((struct sockaddr_in *) &ifcfg->gw)->sin_addr.s_addr;
    netif->broadcast.addr = ((struct sockaddr_in *) &ifcfg->broadcast)->sin_addr.s_addr;

    if (netif->broadcast.addr == IP_ADDR_ANY) {
        netif->broadcast.addr = (netif->ipaddr.addr & netif->netmask.addr) | ~(netif->netmask.addr);
    }

    if (ifcfg->flags & IFCFG_DEFAULT) {
        netif_default = netif;
    } else if (netif == netif_default) {
        netif_default = NULL;
    }

    // Copy hwaddr into ifcfg as info
    memcpy(ifcfg->hwaddr, &netif->hwaddr, sizeof(struct eth_addr));

    // Check for interface up
    if ((ifcfg->flags & IFCFG_UP) == 1 && (netif->flags & NETIF_UP) == 0) {
        netif->flags |= NETIF_UP;

        if (netif->flags & NETIF_DHCP) {
            // Obtain network parameters using DHCP
            state = dhcp_start(netif);
            if (state) {
                if (wait_for_object(&state->binding_complete, 30000) < 0) return -ETIMEDOUT;

                ((struct sockaddr_in *) &ifcfg->addr)->sin_addr.s_addr = netif->ipaddr.addr;
                ((struct sockaddr_in *) &ifcfg->netmask)->sin_addr.s_addr = netif->netmask.addr;
                ((struct sockaddr_in *) &ifcfg->gw)->sin_addr.s_addr = netif->gw.addr;
                ((struct sockaddr_in *) &ifcfg->broadcast)->sin_addr.s_addr = netif->broadcast.addr;
            }
        }
    }

    return 0;
}
