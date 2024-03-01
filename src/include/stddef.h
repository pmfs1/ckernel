//
// Definitions/declarations for common constants, types, variables
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef STDDEF_H
#define STDDEF_H

#include <sys/types.h>

#ifndef _PTRDIFF_T_DEFINED
#define _PTRDIFF_T_DEFINED
typedef int ptrdiff_t;
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef KERNEL
#ifndef errno

#ifdef  __cplusplus
extern "C" {
#endif

osapi int *_errno();

#define errno (*_errno())

#ifdef  __cplusplus
}
#endif

#endif
#endif

#ifndef offsetof
#define offsetof(s, m) ((size_t)&(((s *) 0)->m))
#endif

#endif
