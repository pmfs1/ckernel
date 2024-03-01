//
// Low-level file handling and I/O functions
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef IO_H
#define IO_H

#include <sys/types.h>

#ifdef  __cplusplus
extern "C" {
#endif

osapi handle_t

open(const char *name, int flags, ...);

osapi handle_t

sopen(const char *name, int flags, int shflags, ...);

osapi handle_t

creat(const char *name, int mode);

osapi int close(handle_t
h);

osapi int read(handle_t
f,
void *data, size_t
size);
osapi int write(handle_t
f,
const void *data, size_t
size);

osapi handle_t
dup(handle_t
h);
osapi handle_t
dup2(handle_t
h1,
handle_t h2
);

osapi loff_t
tell(handle_t
f);
osapi loff_t
lseek(handle_t
f,
loff_t offset,
int origin
);
osapi loff_t
filelength(handle_t
f);
osapi off64_t
filelength64(handle_t
f);

osapi int access(const char *name, int mode);

osapi int umask(int mode);

osapi int eof(handle_t
f);
osapi int setmode(handle_t
f,
int mode
);

#ifdef  __cplusplus
}
#endif

#endif
