//
// User context
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef UCONTEXT_H
#define UCONTEXT_H

#include <sys/types.h>

//
// General registers in context
//

#define REG_ES     0
#define REG_DS     1
#define REG_EDI    2
#define REG_ESI    3
#define REG_EBP    4
#define REG_EBX    5
#define REG_EDX    6
#define REG_ECX    7
#define REG_EAX    8
#define REG_TRAPNO 9
#define REG_ERR    10
#define REG_EIP    11
#define REG_ECS    12
#define REG_EFLAGS 13
#define REG_ESP    14
#define REG_ESS    15

#define NGREG      16

//
// Machine-dependent context
//

struct mcontext {
    int gregs[NGREG];
};

typedef struct mcontext mcontext_t;

//
// Stack
//

struct stack {
    void *ss_sp;       // Stack base or pointer
    size_t ss_size;    // Stack size
    int ss_flags;      // Flags
};

typedef struct stack stack_t;

//
// User context
//

struct ucontext {
    struct ucontext *uc_link;
    sigset_t uc_sigmask;
    stack_t uc_stack;
    mcontext_t uc_mcontext;
};

typedef struct ucontext ucontext_t;

#ifdef  __cplusplus
extern "C" {
#endif


#ifdef  __cplusplus
}
#endif

#endif
