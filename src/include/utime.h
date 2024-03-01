//
// Definitions/declarations for utime()
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef UTIME_H
#define UTIME_H

#include <sys/types.h>

#ifndef _UTIMBUF_DEFINED
#define _UTIMBUF_DEFINED

struct utimbuf {
    time_t modtime;
    time_t actime;
    time_t ctime;
};

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi int futime(handle_t f, struct utimbuf *times);

osapi int utime(const char *name, struct utimbuf *times);

#ifdef  __cplusplus
}
#endif

#endif
