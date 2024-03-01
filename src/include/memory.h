//
// Buffer (memory) manipulation routines
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef MEMORY_H
#define MEMORY_H

#include <sys/types.h>

#ifdef  __cplusplus
extern "C" {
#endif

void *memcpy(void *, const void *, size_t);

int memcmp(const void *, const void *, size_t);

void *memset(void *, int, size_t);

void *memmove(void *dst, const void *src, size_t count);

void *memchr(const void *buf, int ch, size_t count);

void *memccpy(void *dst, const void *src, int c, size_t count);

int memicmp(const void *buf1, const void *buf2, size_t count);

#ifdef  __cplusplus
}
#endif

#endif
