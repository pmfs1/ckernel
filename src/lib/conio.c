//
// Console I/O
//
#include <os.h>
#include <string.h>

int cputs(char *string) {
    return write(fdout, string, strlen(string));
}

int getch() {
    unsigned char c;
    int rc;

    rc = read(fdin, &c, 1);
    if (rc != 1) return -1;
    if (c == 0x03) raise(SIGINT);

    return c;
}

int putch(int ch) {
    unsigned char c;
    int rc;

    c = (unsigned char) ch;
    rc = write(fdout, &c, 1);
    if (rc != 1) return -1;

    return ch;
}

int kbhit() {
    return ioctl(fdin, IOCTL_KBHIT, NULL, 0) > 0;
}
