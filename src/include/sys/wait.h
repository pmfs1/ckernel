//
// Wait for child process termination
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_WAIT_H
#define SYS_WAIT_H

#include <sys/types.h>

#define WNOHANG 1

//
// Exit status format:
//
// +------------------------------------------------------------------------+
// |    reserved                | CONT  | STOP  |  SIG  | signum | exitcode |
// |    13 bits                 | 1 bit | 1 bit | 1 bit | 8 bits |  8 bits  |
// +------------------------------------------------------------------------+
//

#define WIFEXITED(status)    (((status) & 0x10000) == 0)
#define WEXITSTATUS(status)  ((status) & 0xFF)
#define WIFSIGNALED(status)  ((status) & 0x10000)
#define WTERMSIG(status)     (((status) >> 8) & 0xFF)
#define WIFSTOPPED(status)   ((status) & 0x20000)
#define WSTOPSIG(status)     (((status) >> 8) & 0xFF)
#define WIFCONTINUED(status) ((status) & 0x40000)

#ifdef  __cplusplus
extern "C" {
#endif

pid_t wait(int *stat_loc);

pid_t waitpid(pid_t pid, int *stat_loc, int options);

#ifdef  __cplusplus
}
#endif

#endif
