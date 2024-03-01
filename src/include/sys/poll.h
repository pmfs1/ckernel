//
// Wait for events on set of file descriptors
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_POLL_H
#define SYS_POLL_H

#include <sys/types.h>

#ifndef _POLL_FD_DEFINED
#define _POLL_FD_DEFINED

struct pollfd {
    int fd;                     // File descriptor
    short events;               // Requested events
    short revents;              // Returned events
};

#define POLLIN      0x0001    // Data may be read without blocking
#define POLLPRI     0x0002    // High priority data may be read without blocking
#define POLLOUT     0x0004    // Data may be written without blocking
#define POLLERR     0x0008    // An error has occurred (revents only)
#define POLLHUP     0x0010    // Device has been disconnected (revents only)
#define POLLNVAL    0x0020    // Invalid fd member (revents only)

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi int poll(struct pollfd fds[], unsigned int nfds, int timeout);

#ifdef  __cplusplus
}
#endif

#endif
