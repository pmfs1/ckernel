//
// Assert macro
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef ASSERT_H
#define ASSERT_H

#ifdef NDEBUG

#ifdef  __cplusplus
extern "C" {
#endif

void _assert(void *expr, void *filename, unsigned lineno);

#ifdef  __cplusplus
}
#endif

#define assert(exp) (void) ((exp) || (_assert(#exp, __FILE__, __LINE__), 0))

#else

#define assert(exp) ((void) 0)

#endif

#endif
