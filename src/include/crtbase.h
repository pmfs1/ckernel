//
// Internal definitions for C runtime library
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef CRTBASE_H
#define CRTBASE_H

#include <stdio.h>

struct opt {
    int err;
    int ind;
    int opt;
    char *arg;
    int sp;
    int reset;
    char *place;
};

struct crtbase {
    int argc;
    char **argv;
    int stdio_init;
    int stdio_initialized;
    FILE iob[3];
    char stdinbuf[BUFSIZ];
    struct opt opt;

    void (*fork_exit)(int);
};

#endif
