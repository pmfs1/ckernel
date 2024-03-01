//
// Shell command library
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SHLIB_H
#define SHLIB_H

#include <stdio.h>

typedef int (*main_t)(int argc, char *argv[]);

#ifdef SHELL
#define shellcmd(name) __declspec(dllexport) int cmd_##name(int argc, char *argv[])
#else
#define shellcmd(name) int main(int argc, char *argv[])
#endif

#ifdef  __cplusplus
extern "C" {
#endif

char *join_path(char *dir, char *name);

char *get_symbolic_mode(int mode, char buf[11]);

int parse_symbolic_mode(char *symbolic, int orig);

int parse_url(char *url, char **host, int *port, char **path);

FILE *open_url(char *url, char *agent);

#ifdef  __cplusplus
}
#endif

#endif
