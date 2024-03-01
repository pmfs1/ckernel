//
// File locking
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_FILE_H
#define SYS_FILE_H

#include <sys/types.h>

#define LOCK_SH     1  // Shared lock
#define LOCK_EX     2  // Exclusive lock
#define LOCK_UN     3  // Remove lock

#ifdef  __cplusplus
extern "C" {
#endif

int flock(handle_t f, int operation);

#ifdef  __cplusplus
}
#endif

#endif
