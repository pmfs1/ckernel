//
// User database
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef PWD_H
#define PWD_H

#include <sys/types.h>

#ifndef _PASSWD_DEFINED
#define _PASSWD_DEFINED

struct passwd {
    char *pw_name;    // User name
    char *pw_passwd;  // User password
    uid_t pw_uid;     // User id
    gid_t pw_gid;     // Group id
    char *pw_gecos;   // Real name
    char *pw_dir;     // Home directory
    char *pw_shell;   // Shell program
};

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi struct passwd *getpwnam(const char *name);

osapi struct passwd *getpwuid(uid_t uid);

#ifdef  __cplusplus
}
#endif

#endif
