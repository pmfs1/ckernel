//
// Terminal control interface
//
#include <os.h>
#include <termios.h>

int cfsetispeed(struct termios *termios, speed_t speed) {
    errno = ENOSYS;
    return -1;
}

int cfsetospeed(struct termios *termios, speed_t speed) {
    errno = ENOSYS;
    return -1;
}


speed_t cfgetispeed(struct termios *termios) {
    errno = ENOSYS;
    return -1;
}

speed_t cfgetospeed(struct termios *termios) {
    errno = ENOSYS;
    return -1;
}

int tcdrain(handle_t f) {
    errno = ENOSYS;
    return -1;
}

int tcflow(handle_t f, int action) {
    errno = ENOSYS;
    return -1;
}

int tcflush(handle_t f, int control) {
    errno = ENOSYS;
    return -1;
}

int tcsendbreak(handle_t f, int duration) {
    errno = ENOSYS;
    return -1;
}


int tcgetattr(handle_t f, struct termios *termios) {
    errno = ENOSYS;
    return -1;
}

int tcsetattr(handle_t f, int flag, struct termios *termios) {
    errno = ENOSYS;
    return -1;
}

int tcgetsize(handle_t f, int *rows, int *cols) {
    errno = ENOSYS;
    return -1;
}
