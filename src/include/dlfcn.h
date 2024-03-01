//
// Dynamic linking
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef DLFCN_H
#define DLFCN_H

#include <sys/types.h>

#ifndef RTLD_LAZY

#define RTLD_LAZY     0x0001
#define RTLD_NOW      0x0002
#define RTLD_GLOBAL   0x0100
#define RTLD_LOCAL    0x0000
#define RTLD_NOSHARE  0x1000
#define RTLD_EXE      0x2000
#define RTLD_SCRIPT   0x4000

#define RTLD_DEFAULT ((void *) 0)

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi hmodule_t

dlopen(const char *name, int mode);

osapi int dlclose(hmodule_t
hmod);
osapi void *dlsym(hmodule_t
hmod,
const char *procname
);

osapi char *dlerror();

#ifdef  __cplusplus
}
#endif

#endif
