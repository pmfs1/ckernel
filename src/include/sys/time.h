//
// Time types
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_TIME_H
#define SYS_TIME_H

#include <sys/types.h>

#ifndef _TIMEVAL_DEFINED
#define _TIMEVAL_DEFINED

struct timeval {
    long tv_sec;                  // Seconds
    long tv_usec;                 // Microseconds
};

#endif

struct itimerval {
    struct timeval it_interval;   // Timer interval
    struct timeval it_value;      // Current value
};

#define ITIMER_REAL    0           // A SIGALRM signal is delivered when this timer expires
#define ITIMER_VIRTUAL 1           // A SIGVTALRM signal is delivered when this timer expires
#define ITIMER_PROF    2           // A SIGPROF signal is delivered when this timer expires

#ifdef  __cplusplus
extern "C" {
#endif

int getitimer(int which, struct itimerval *value);

int setitimer(int which, const struct itimerval *value, struct itimerval *oldvalue);

osapi int gettimeofday(struct timeval *tv, void *tzp);

#ifdef  __cplusplus
}
#endif

#endif
