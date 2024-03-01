//
// Select from file descriptor set
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_SELECT_H
#define SYS_SELECT_H

#include <sys/types.h>

#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED

struct timeval {
    long tv_sec;                  // Seconds
    long tv_usec;                 // Microseconds
};

#endif

#ifndef FD_SETSIZE
#define FD_SETSIZE 64
#endif

#ifndef _FD_SET_DEFINED
#define _FD_SET_DEFINED

typedef struct fd_set {
    unsigned int count;
    int fd[FD_SETSIZE];
} fd_set;

#endif

#define FD_ZERO(set) _fd_zero(set)
#define FD_ISSET(fd, set) _fd_isset(fd, set)
#define FD_SET(fd, set) _fd_set(fd, set)
#define FD_CLR(fd, set) _fd_clr(fd, set)

#ifndef _FD_FUNCS_DEFINED
#define _FD_FUNCS_DEFINED

__inline void _fd_zero(fd_set *set) {
    set->count = 0;
}

__inline int _fd_isset(int fd, fd_set *set) {
    unsigned int i;

    for (i = 0; i < set->count; i++) if (set->fd[i] == fd) return 1;
    return 0;
}

__inline void _fd_set(int fd, fd_set *set) {
    if (set->count < FD_SETSIZE) set->fd[set->count++] = fd;
}

__inline void _fd_clr(int fd, fd_set *set) {
    unsigned int i;

    for (i = 0; i < set->count; i++) {
        if (set->fd[i] == fd) {
            while (i < set->count - 1) {
                set->fd[i] = set->fd[i + 1];
                i++;
            }
            set->count--;
            break;
        }
    }
}

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, const struct timeval *timeout);

#ifdef  __cplusplus
}
#endif

#endif
