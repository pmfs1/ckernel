//
// Definitions for network database operations
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef NETDB_H
#define NETDB_H

#include <sys/types.h>

#ifndef _IN_PORT_T_DEFINED
#define _IN_PORT_T_DEFINED
typedef unsigned short in_port_t;
#endif

#ifndef _IN_ADDR_T_DEFINED
#define _IN_ADDR_T_DEFINED
typedef unsigned long in_addr_t;
#endif

#ifndef _HOSTENT_DEFINED
#define _HOSTENT_DEFINED

struct hostent {
    char *h_name;        // Official name of host
    char **h_aliases;    // Alias list
    short h_addrtype;    // Host address type
    short h_length;      // Length of address
    char **h_addr_list;  // List of addresses
};

#define h_addr h_addr_list[0]

#endif

#ifndef _PROTOENT_DEFINED
#define _PROTOENT_DEFINED

struct protoent {
    char *p_name;
    char **p_aliases;
    short p_proto;
};

#endif

#ifndef _SERVENT_DEFINED
#define _SERVENT_DEFINED

struct servent {
    char *s_name;
    char **s_aliases;
    short s_port;
    char *s_proto;
};

#endif

#define HOST_NOT_FOUND  82
#define TRY_AGAIN       83
#define NO_RECOVERY     84
#define NO_DATA         85

#ifdef  __cplusplus
extern "C" {
#endif

osapi struct hostent *gethostbyname(const char *name);

osapi struct hostent *gethostbyaddr(const char *addr, int len, int type);

osapi struct protoent *getprotobyname(const char *name);

osapi struct protoent *getprotobynumber(int proto);

osapi struct servent *getservbyname(const char *name, const char *proto);

osapi struct servent *getservbyport(int port, const char *proto);

#define hstrerror strerror
#define h_errno errno

#ifdef  __cplusplus
}
#endif

#endif
