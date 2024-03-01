//
// Non-local goto
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SETJMP_H
#define SETJMP_H

#include <sys/types.h>

typedef struct {
    unsigned long ebp;
    unsigned long ebx;
    unsigned long edi;
    unsigned long esi;
    unsigned long esp;
    unsigned long eip;
} jmp_buf[1];

typedef struct {
    jmp_buf env;
    sigset_t sigmask;
} sigjmp_buf[1];

#define _setjmp setjmp
#define _longjmp longjmp

#ifdef  __cplusplus
extern "C" {
#endif

int setjmp(jmp_buf env);

void longjmp(jmp_buf env, int value);

int sigsetjmp(sigjmp_buf env, int savesigs);

void siglongjmp(sigjmp_buf env, int value);

#ifdef  __cplusplus
}
#endif

#endif
