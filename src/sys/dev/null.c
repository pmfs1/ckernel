//
// Null device driver
//
#include <os/krnl.h>

dev_t nulldev = NODEV;

static int null_ioctl(struct dev *dev, int cmd, void *args, size_t size) {
    switch (cmd) {
        case IOCTL_GETDEVSIZE:
            return 0;

        case IOCTL_GETBLKSIZE:
            return 1;
    }

    return -1;
}

static int null_read(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    return 0;
}

static int null_write(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    return count;
}

struct driver null_driver = {
        "null",
        DEV_TYPE_STREAM,
        null_ioctl,
        null_read,
        null_write
};

int __declspec(dllexport) null() {
    dev_make("null", &null_driver, NULL, NULL);
    nulldev = dev_open("null");
    return 0;
}
