//
// Definitions for resource operations
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_RESOURCE_H
#define SYS_RESOURCE_H

#include <sys/types.h>

#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED

struct timeval {
    long tv_sec;                  // Seconds
    long tv_usec;                 // Microseconds
};

#endif

#define RUSAGE_SELF -1

struct rusage {
    struct timeval ru_utime;      // User time used
    struct timeval ru_stime;      // System time used
};

#ifdef  __cplusplus
extern "C" {
#endif

int getrusage(int who, struct rusage *usage);

#ifdef  __cplusplus
}
#endif

#endif
