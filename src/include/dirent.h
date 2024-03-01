//
// List directory entries
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef DIRENT_H
#define DIRENT_H

#include <sys/types.h>

#define NAME_MAX   255

struct dirent {
    ino_t d_ino;
    int d_namlen;
    char d_name[NAME_MAX + 1];
};

typedef struct {
    int handle;
    char path[NAME_MAX + 1];
    struct dirent entry;
} DIR;

#ifdef  __cplusplus
extern "C" {
#endif

DIR *opendir(const char *name);

int closedir(DIR *dirp);

struct dirent *readdir(DIR *dirp);

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);

int rewinddir(DIR *dirp);

#ifdef  __cplusplus
}
#endif

#endif
