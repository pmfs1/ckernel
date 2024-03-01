//
// POSIX scheduling library
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SCHED_H
#define SCHED_H

#include <sys/types.h>

#define SCHED_OTHER 0
#define SCHED_FIFO  1
#define SCHED_RR    2

#define SCHED_MIN   SCHED_OTHER
#define SCHED_MAX   SCHED_RR

#ifndef _SCHED_PARAM_DEFINED
#define _SCHED_PARAM_DEFINED
struct sched_param {
    int sched_priority;
};
#endif

#ifdef  __cplusplus
extern "C" {
#endif

int sched_yield(void);

int sched_get_priority_min(int policy);

int sched_get_priority_max(int policy);

int sched_setscheduler(pid_t pid, int policy);

int sched_getscheduler(pid_t pid);

#ifdef  __cplusplus
}
#endif

#endif
