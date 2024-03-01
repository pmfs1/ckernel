//
// Process times
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_TIMES_H
#define SYS_TIMES_H

#include <sys/types.h>

#ifndef _TMS_DEFINED
#define _TMS_DEFINED

struct tms {
    clock_t tms_utime;  // User CPU time
    clock_t tms_stime;  // System CPU time
    clock_t tms_cutime; // User CPU time of terminated child threads
    clock_t tms_cstime; // System CPU time of terminated child threads
};

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi clock_t
threadtimes(handle_t
thread,
struct tms *tms
);
osapi clock_t

times(struct tms *tms);

#ifdef  __cplusplus
}
#endif

#endif
