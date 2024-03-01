//
// Heap allocation routines
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef MALLOC_H
#define MALLOC_H

#include <sys/types.h>

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef USE_LOCAL_HEAP

osapi void *_lmalloc(size_t size);
osapi void *_lrealloc(void *mem, size_t size);
osapi void *_lcalloc(size_t num, size_t size);
osapi void _lfree(void *p);

#define malloc(n) _lmalloc(n)
#define realloc(p, n) _lrealloc((p), (n))
#define calloc(n, s) _lcalloc((n), (s))
#define free(p) _lfree(p)

#else

osapi void *malloc(size_t size);

osapi void *realloc(void *mem, size_t size);

osapi void *calloc(size_t num, size_t size);

osapi void free(void *p);

#endif

void *_alloca(size_t size);

#define alloca _alloca

#ifdef  __cplusplus
}
#endif

#endif
