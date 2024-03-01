//
// RAM disk driver
//
#include <os/krnl.h>

struct ramdisk {
    unsigned int blks;
    char *data;
};

static int ramdisk_ioctl(struct dev *dev, int cmd, void *args, size_t size) {
    struct ramdisk *rd = (struct ramdisk *) dev->privdata;

    switch (cmd) {
        case IOCTL_GETDEVSIZE:
            return rd->blks;

        case IOCTL_GETBLKSIZE:
            return SECTORSIZE;
    }

    return -ENOSYS;
}

static int ramdisk_read(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    struct ramdisk *rd = (struct ramdisk *) dev->privdata;

    if (count == 0) return 0;
    if (blkno + count / SECTORSIZE > rd->blks) {
        count = (rd->blks - blkno) * SECTORSIZE;
    }
    memcpy(buffer, rd->data + blkno * SECTORSIZE, count);
    return count;
}

static int ramdisk_write(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    struct ramdisk *rd = (struct ramdisk *) dev->privdata;

    if (count == 0) return 0;
    if (blkno + count / SECTORSIZE > rd->blks) return -EFAULT;
    memcpy(rd->data + blkno * SECTORSIZE, buffer, count);
    return count;
}

struct driver ramdisk_driver = {
        "ramdisk",
        DEV_TYPE_BLOCK,
        ramdisk_ioctl,
        ramdisk_read,
        ramdisk_write
};

int __declspec(dllexport) ramdisk(struct unit *unit, char *opts) {
    struct ramdisk *rd;
    char devname[DEVNAMELEN];
    int size;
    dev_t devno;
    char *data;

    get_option(opts, "devname", devname, DEVNAMELEN, "rd#");
    size = get_num_option(opts, "size", 1440) * 1024;

    data = kmalloc(size);
    if (!data) return -ENOMEM;

    rd = kmalloc(sizeof(struct ramdisk));
    if (!rd) return -ENOMEM;

    rd->blks = size / SECTORSIZE;
    rd->data = data;

    devno = dev_make(devname, &ramdisk_driver, NULL, rd);

    kprintf(KERN_INFO
    "%s: ramdisk (%d KB)\n", device(devno)->name, size / 1024);
    return 0;
}

int create_initrd() {
    struct ramdisk *rd;
    dev_t devno;
    int size;

    size = syspage->ldrparams.initrd_size;
    if (size == 0) return 0;

    rd = kmalloc(sizeof(struct ramdisk));
    if (!rd) return -ENOMEM;

    rd->blks = size / SECTORSIZE;
    rd->data = (char *) INITRD_ADDRESS;

    devno = dev_make("initrd", &ramdisk_driver, NULL, rd);

    kprintf(KERN_INFO
    "%s: ramdisk (%d KB)\n", device(devno)->name, size / 1024);
    return 0;
}
