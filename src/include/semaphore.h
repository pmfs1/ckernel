//
// POSIX semaphore library
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <sys/types.h>

#ifndef _TIMESPEC_DEFINED
#define _TIMESPEC_DEFINED
struct timespec {
    long tv_sec;
    long tv_nsec;
};
#endif

#ifndef _SEM_T_DEFINED
#define _SEM_T_DEFINED
typedef handle_t sem_t;
#endif

#define _POSIX_SEMAPHORES

#define SEM_FAILED (-1)

#ifdef  __cplusplus
extern "C" {
#endif

int sem_init(sem_t *sem, int pshared, unsigned int value);

int sem_destroy(sem_t *sem);

int sem_trywait(sem_t *sem);

int sem_wait(sem_t *sem);

int sem_timedwait(sem_t *sem, const struct timespec *abstime);

int sem_post(sem_t *sem);

int sem_post_multiple(sem_t *sem, int count);

int sem_open(const char *name, int oflag, int mode, unsigned int value);

int sem_close(sem_t *sem);

int sem_unlink(const char *name);

int sem_getvalue(sem_t *sem, int *sval);

#ifdef  __cplusplus
}
#endif

#endif
