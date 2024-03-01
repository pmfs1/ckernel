//
// System Management BIOS (SMBIOS) driver
//
#include <os/krnl.h>

//
// SMBIOS Entry Point Structure
//

#pragma pack(push, 1)

struct smbios_eps {
    unsigned char anchor[4]; // '_SM_'
    unsigned char checksum;
    unsigned char length;

    unsigned char smbios_major;
    unsigned char smbios_minor;
    unsigned short max_size;
    unsigned char revision;
    unsigned char formatted_area[5];

    unsigned char intermediate_anchor[5]; // '_DMI_'
    unsigned char intermediate_checksum;

    unsigned short structure_table_length;
    unsigned long structure_table_address;
    unsigned short structure_count;

    unsigned char smbios_bcd_revision;
};

#pragma pack(pop)

#define IOCTL_SMBIOS_GETEPS       1024

static struct smbios_eps *eps = NULL;
static char *smbios_table = NULL;

static int init_smbios() {
    unsigned char *biosarea;
    int ofs, n;
    struct smbios_eps *s;
    unsigned char chksum;
    int pgofs;

    // Search the bios data area (0xf0000-0xffff0) for a valid SMBIOS structure.
    biosarea = iomap(0xF0000, 0x10000);
    if (!biosarea) return -EIO;
    for (ofs = 0x0000; ofs < 0xFFF0; ofs += 0x10) {
        s = (struct smbios_eps *) (biosarea + ofs);

        // Check _SM_ signature
        if (s->anchor[0] != '_' || s->anchor[1] != 'S' || s->anchor[2] != 'M' || s->anchor[3] != '_') continue;

        // Check structure checksum
        if (!s->length) continue;
        chksum = 0;
        for (n = 0; n < s->length; n++) chksum += biosarea[ofs + n];
        if (chksum != 0) continue;

        //kprintf("smbios: SMBIOS %d.%d EPS found at 0x%08x\n", s->smbios_major, s->smbios_minor, 0xF0000 + ofs);
        //kprintf("smbios: table addr=0x%08x len=%d\n", s->structure_table_address, s->structure_table_length);

        // Make a copy of SMBIOS entry point structure
        eps = kmalloc(s->length);
        if (!eps) return -ENOMEM;
        memcpy(eps, s, s->length);

        // Map the SMBIOS structure table
        pgofs = PGOFF(eps->structure_table_address);
        smbios_table = iomap(eps->structure_table_address - pgofs, eps->structure_table_length + pgofs);
        if (!smbios_table) return -EIO;
        smbios_table += pgofs;

        break;
    }

    iounmap(biosarea, 0x10000);
    return 0;
}

static int smbios_ioctl(struct dev *dev, int cmd, void *args, size_t size) {
    switch (cmd) {
        case IOCTL_GETDEVSIZE:
            return eps ? eps->structure_table_length : 0;

        case IOCTL_GETBLKSIZE:
            return 1;

        case IOCTL_SMBIOS_GETEPS:
            if (!eps) return -EIO;
            if (!args || size < eps->length) return -EINVAL;
            memcpy(args, eps, eps->length);
            return eps->length;
    }

    return -ENOSYS;
}

static int smbios_read(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    if (count == 0) return 0;
    if (!eps || !smbios_table) return -EIO;
    if (blkno + count > eps->structure_table_length) return -EFAULT;
    memcpy(buffer, smbios_table + blkno, count);
    return count;
}

static int smbios_write(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    return -ENOSYS;
}

struct driver smbios_driver = {
        "smbios",
        DEV_TYPE_BLOCK,
        smbios_ioctl,
        smbios_read,
        smbios_write
};

int __declspec(dllexport) smbios(struct unit *unit) {
    int rc;

    rc = init_smbios();
    if (rc < 0) return rc;

    dev_make("smbios", &smbios_driver, NULL, NULL);
    return 0;
}
