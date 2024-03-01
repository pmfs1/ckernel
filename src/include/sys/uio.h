//
// Defines for vector I/O functions
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_UIO_H
#define SYS_UIO_H

#include <sys/types.h>

#ifndef _IOVEC_DEFINED
#define _IOVEC_DEFINED

struct iovec {
    size_t iov_len;
    void *iov_base;
};

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi int readv(handle_t f, const struct iovec *iov, int count);

osapi int writev(handle_t f, const struct iovec *iov, int count);

#ifdef  __cplusplus
}
#endif

#endif
