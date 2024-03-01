//
// Shell
//
#ifndef SH_H
#define SH_H

#include <os.h>
#include <crtbase.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <inifile.h>
#include <glob.h>
#include <fnmatch.h>
#include <libgen.h>
#include <shlib.h>

#include "stmalloc.h"
#include "input.h"
#include "chartype.h"
#include "node.h"
#include "parser.h"
#include "job.h"
#include "interp.h"

#define MAX_COMMAND_LEN 8
#define MAX_INTSTR      12

typedef int (*builtin_t)(struct job *job);

#define builtin(name) __declspec(dllexport) int builtin_##name(struct job *job)

//
// Function definition
//

struct function {
    union node *def;
    struct stkmark mark;
    struct function *next;
};

//
// Shell
//

struct shell {
    struct job *top;
    struct job *jobs;
    struct function *funcs;
    int fd[STD_HANDLES];
    int lastpid;
    int lastrc;
    int done;
    int debug;
    struct term *term;
};

#endif
