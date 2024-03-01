//
// User group database
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef GRP_H
#define GRP_H

#include <sys/types.h>

#ifndef _GROUP_DEFINED
#define _GROUP_DEFINED

struct group {
    char *gr_name;    // Group name
    char *gr_passwd;  // Group password
    gid_t gr_gid;     // Group id
    char **gr_mem;    // Group members
};

#endif

#ifdef  __cplusplus
extern "C" {
#endif

osapi struct group *getgrnam(const char *name);

osapi struct group *getgrgid(uid_t uid);

osapi int setgroups(int size, const gid_t list[]);

osapi int initgroups(const char *user, gid_t basegid);

#ifdef  __cplusplus
}
#endif

#endif
