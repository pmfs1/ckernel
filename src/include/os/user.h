//
// User management
//
#ifndef USER_H
#define USER_H

int getuid();

int setuid(uid_t uid);

int getgid();

int setgid(gid_t gid);

int geteuid();

int seteuid(uid_t uid);

int getegid();

int setegid(gid_t gid);

int setgroups(int size, gid_t *list);

int getgroups(int size, gid_t *list);

int check(int mode, uid_t uid, gid_t gid, int access);

#endif
