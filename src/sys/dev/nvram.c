//
// NVRAM driver
//
#include <os/krnl.h>

#define NVRAM_SIZE 128

static int nvram_ioctl(struct dev *dev, int cmd, void *args, size_t size) {
    switch (cmd) {
        case IOCTL_GETDEVSIZE:
            return NVRAM_SIZE;

        case IOCTL_GETBLKSIZE:
            return 1;
    }

    return -ENOSYS;
}

static int nvram_read(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    unsigned int n;

    if (count == 0) return 0;
    if (blkno + count > NVRAM_SIZE) return -EFAULT;

    for (n = 0; n < count; n++) ((unsigned char *) buffer)[n] = read_cmos_reg(n + blkno);
    return count;
}

static int nvram_write(struct dev *dev, void *buffer, size_t count, blkno_t blkno, int flags) {
    unsigned int n;

    if (count == 0) return 0;
    if (blkno + count > NVRAM_SIZE) return -EFAULT;

    for (n = 0; n < count; n++) write_cmos_reg(n + blkno, ((unsigned char *) buffer)[n]);
    return count;
}

struct driver nvram_driver = {
        "nvram",
        DEV_TYPE_BLOCK,
        nvram_ioctl,
        nvram_read,
        nvram_write
};

int __declspec(dllexport) nvram() {
    dev_make("nvram", &nvram_driver, NULL, NULL);
    return 0;
}
