//
// Device Filesystem
//
#ifndef DEVFS_H
#define DEVFS_H

struct devfile {
    struct file *filp;
    dev_t devno;
    int blksize;
    int devsize;

    struct devfile *next;
    struct devfile *prev;
};

void init_devfs();

void devfs_setevt(struct dev *dev, int events);

void devfs_clrevt(struct dev *dev, int events);

#endif
