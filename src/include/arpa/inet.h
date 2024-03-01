//
// Definitions for internet operations
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef ARPA_INET_H
#define ARPA_INET_H

#include <sys/types.h>

#ifndef _IN_PORT_T_DEFINED
#define _IN_PORT_T_DEFINED
typedef unsigned short in_port_t;
#endif

#ifndef _IN_ADDR_T_DEFINED
#define _IN_ADDR_T_DEFINED
typedef unsigned long in_addr_t;
#endif

#ifndef _IN_ADDR_DEFINED
#define _IN_ADDR_DEFINED

struct in_addr {
    union {
        struct {
            unsigned char s_b1, s_b2, s_b3, s_b4;
        } s_un_b;
        struct {
            unsigned short s_w1, s_w2;
        } s_un_w;
        unsigned long s_addr;
    };
};

#endif

#ifndef _XTONX_DEFINED
#define _XTONX_DEFINED

__inline unsigned short htons(unsigned short n) {
    return ((n & 0xFF) << 8) | ((n & 0xFF00) >> 8);
}

__inline unsigned short ntohs(unsigned short n) {
    return ((n & 0xFF) << 8) | ((n & 0xFF00) >> 8);
}

__inline unsigned long htonl(unsigned long n) {
    return ((n & 0xFF) << 24) | ((n & 0xFF00) << 8) | ((n & 0xFF0000) >> 8) | ((n & 0xFF000000) >> 24);
}

__inline unsigned long ntohl(unsigned long n) {
    return ((n & 0xFF) << 24) | ((n & 0xFF00) << 8) | ((n & 0xFF0000) >> 8) | ((n & 0xFF000000) >> 24);
}

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi unsigned long inet_addr(const char *cp);

osapi char *inet_ntoa(struct in_addr in);

#ifdef  __cplusplus
}
#endif

#endif
