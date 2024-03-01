//
// System name
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_UTSNAME_H
#define SYS_UTSNAME_H

#include <sys/types.h>

#define UTSNAMELEN 65

#ifndef _UTSNAME_DEFINED
#define _UTSNAME_DEFINED
struct utsname {
    char sysname[UTSNAMELEN];
    char nodename[UTSNAMELEN];
    char release[UTSNAMELEN];
    char version[UTSNAMELEN];
    char machine[UTSNAMELEN];
};
#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi int uname(struct utsname *buf);

#ifdef  __cplusplus
}
#endif

#endif
