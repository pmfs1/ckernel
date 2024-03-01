//
// Socket definitions
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_SOCKET_H
#define SYS_SOCKET_H

#include <sys/types.h>

#ifndef _IOVEC_DEFINED
#define _IOVEC_DEFINED

struct iovec {
    size_t iov_len;
    void *iov_base;
};

#endif

#ifndef _SOCKADDR_DEFINED
#define _SOCKADDR_DEFINED

struct sockaddr {
    unsigned char sa_len;
    unsigned char sa_family;
    char sa_data[14];
};

#endif

#ifndef _MSGHDR_DEFINED
#define _MSGHDR_DEFINED

struct msghdr {
    struct sockaddr *msg_name;
    int msg_namelen;
    struct iovec *msg_iov;
    int msg_iovlen;
};

#endif

#ifndef _LINGER_DEFINED
#define _LINGER_DEFINED

struct linger {
    unsigned short l_onoff;
    unsigned short l_linger;
};

#endif

#ifndef _SOCKLEN_T_DEFINED
#define _SOCKLEN_T_DEFINED
typedef int socklen_t;
#endif

#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

#define SOL_SOCKET      0xffff

#define SO_REUSEADDR    0x0004
#define SO_KEEPALIVE    0x0008
#define SO_BROADCAST    0x0020
#define SO_SNDTIMEO     0x1005
#define SO_RCVTIMEO     0x1006
#define SO_LINGER       0x0080

#define AF_UNSPEC       0
#define AF_INET         2

#define SHUT_RD         0x00
#define SHUT_WR         0x01
#define SHUT_RDWR       0x02

#ifdef  __cplusplus
extern "C" {
#endif

osapi int accept(int s, struct sockaddr *addr, int *addrlen);

osapi int bind(int s, const struct sockaddr *name, int namelen);

osapi int connect(int s, const struct sockaddr *name, int namelen);

osapi int getpeername(int s, struct sockaddr *name, int *namelen);

osapi int getsockname(int s, struct sockaddr *name, int *namelen);

osapi int getsockopt(int s, int level, int optname, void *optval, int *optlen);

osapi int listen(int s, int backlog);

osapi int recv(int s, void *data, int size, unsigned int flags);

osapi int recvfrom(int s, void *data, int size, unsigned int flags, struct sockaddr *from, int *fromlen);

osapi int recvmsg(int s, struct msghdr *hdr, unsigned int flags);

osapi int send(int s, const void *data, int size, unsigned int flags);

osapi int sendto(int s, const void *data, int size, unsigned int flags, const struct sockaddr *to, int tolen);

osapi int sendmsg(int s, struct msghdr *hdr, unsigned int flags);

osapi int setsockopt(int s, int level, int optname, const void *optval, int optlen);

osapi int shutdown(int s, int how);

osapi int socket(int domain, int type, int protocol);

#ifdef  __cplusplus
}
#endif

#endif
