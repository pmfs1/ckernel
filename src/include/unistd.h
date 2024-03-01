//
// Standard symbolic constants and types
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef UNISTD_H
#define UNISTD_H

#include <sys/types.h>

#ifndef NOVFORK

#include <setjmp.h>
#include <alloca.h>

#endif

//
// Values for the second argument to access
//

#ifndef R_OK

#define R_OK    4               // Test for read permission
#define W_OK    2               // Test for write permission
#define X_OK    1               // Test for execute permission
#define F_OK    0               // Test for existence

#endif

//
// Seek types
//

#ifndef SEEK_SET

#define SEEK_SET       0       // Seek relative to begining of file
#define SEEK_CUR       1       // Seek relative to current file position
#define SEEK_END       2       // Seek relative to end of file

#endif

//
// Locking functions
//

#define F_ULOCK        1       // Unlock locked sections
#define F_LOCK         2       // Lock a section for exclusive use
#define F_TLOCK        3       // Test and lock a section for exclusive use
#define F_TEST         4       // Test a section for locks by other processes

//
// Context for vfork()
//

#ifndef NOVFORK

struct _forkctx {
    struct _forkctx *prev;
    int pid;
    jmp_buf jmp;
    int fd[3];
    char **env;
};

#endif

typedef unsigned int useconds_t;

#ifdef  __cplusplus
extern "C" {
#endif

#ifdef LARGEFILES
#define lseek(f, offset, origin) lseek64((f), (offset), (origin))
#define ftruncate(f, size) ftruncate64((f), (size))
#else
osapi loff_t
lseek(handle_t
f,
loff_t offset,
int origin
);
osapi int ftruncate(handle_t
f,
loff_t size
);
#endif

osapi int ftruncate64(handle_t
f,
off64_t size
);
osapi off64_t
lseek64(handle_t
f,
off64_t offset,
int origin
);

osapi int access(const char *name, int mode);

osapi int close(handle_t
h);
osapi int fsync(handle_t
f);
osapi int read(handle_t
f,
void *data, size_t
size);
osapi int write(handle_t
f,
const void *data, size_t
size);
#ifdef LARGEFILES
osapi int pread(handle_t f, void *data, size_t size, off64_t offset);
osapi int pwrite(handle_t f, const void *data, size_t size, off64_t offset);
#endif
osapi int pipe(handle_t
fildes[2]);

osapi int chdir(const char *name);

osapi char *getcwd(char *buf, size_t size);

osapi handle_t
dup(handle_t
h);
osapi handle_t
dup2(handle_t
h1,
handle_t h2
);

osapi int link(const char *oldname, const char *newname);

osapi int unlink(const char *name);

osapi int mkdir(const char *name, int mode);

osapi int rmdir(const char *name);

osapi int chown(const char *name, int owner, int group);

osapi int fchown(handle_t
f,
int owner,
int group
);
osapi int isatty(handle_t
f);

int readlink(const char *name, char *buf, size_t size);

int symlink(const char *oldpath, const char *newpath);

int chroot(const char *path);

int lockf(handle_t
f,
int func, off_t
size);

osapi int gethostname(char *name, int namelen);

osapi int getpagesize();

osapi void _exit(int status);

osapi unsigned sleep(unsigned seconds);

int usleep(useconds_t usec);

osapi unsigned alarm(unsigned seconds);

osapi char *crypt(const char *key, const char *salt);

osapi pid_t

getpid();

osapi pid_t

getppid();

osapi int getuid();

osapi int getgid();

osapi int setuid(uid_t uid);

osapi int setgid(gid_t gid);

osapi int geteuid();

osapi int getegid();

osapi int seteuid(uid_t uid);

osapi int setegid(gid_t gid);

osapi int getgroups(int size, gid_t list[]);

osapi int __getstdhndl(int n);

#define STDIN_FILENO  __getstdhndl(0)
#define STDOUT_FILENO __getstdhndl(1)
#define STDERR_FILENO __getstdhndl(2)

#ifndef NOGETOPT
#ifndef _GETOPT_DEFINED
#define _GETOPT_DEFINED

int *_opterr();

int *_optind();

int *_optopt();

char **_optarg();

#define opterr (*_opterr())
#define optind (*_optind())
#define optopt (*_optopt())
#define optarg (*_optarg())

int getopt(int argc, char **argv, char *opts);

#endif
#endif

#ifndef NOVFORK

struct _forkctx *_vfork(struct _forkctx *fc);

int execve(const char *path, char *argv[], char *env[]);

int execv(const char *path, char *argv[]);

int execl(const char *path, char *arg0, ...);

#define vfork() (setjmp(_vfork((struct _forkctx *) alloca(sizeof(struct _forkctx)))->jmp))

#endif

#ifndef environ

osapi char ***_environ();

#define environ (*_environ())
#endif

#ifdef  __cplusplus
}
#endif

#endif
