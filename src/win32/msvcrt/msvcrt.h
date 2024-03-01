//
// C runtime library
//
#ifndef MSVCRT_H
#define MSVCRT_H

#include <os.h>
#include <stdarg.h>
#include <ctype.h>
#include <inifile.h>
#include <win32.h>

// Application types

#define _UNKNOWN_APP    0
#define _CONSOLE_APP    1
#define _GUI_APP        2

// I/O stream flags

#define _IOREAD         0x0001
#define _IOWRT          0x0002

#define _IOFBF          0x0000
#define _IOLBF          0x0040
#define _IONBF          0x0004

#define _IOMYBUF        0x0008
#define _IOEOF          0x0010
#define _IOERR          0x0020
#define _IOSTRG         0x0040
#define _IORW           0x0080

#define _IOAPPEND       0x0200

#define _IOYOURBUF      0x0100
#define _IOSETVBUF      0x0400
#define _IOFEOF         0x0800
#define _IOFLRTN        0x1000
#define _IOCTRLZ        0x2000
#define _IOCOMMIT       0x4000

#define _IOFREE         0x10000

// File modes (fmode)

#define _O_TEXT         0x4000  // File mode is text (translated)
#define _O_BINARY       0x8000  // File mode is binary (untranslated)

// Math exceptions

#define _DOMAIN     1   // Argument domain error
#define _SING       2   // Argument singularity
#define _OVERFLOW   3   // Overflow range error
#define _UNDERFLOW  4   // Underflow range error
#define _TLOSS      5   // Total loss of precision
#define _PLOSS      6   // Partial loss of precision

#ifndef _SIZE_T_DEFINED
#define _SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

#ifndef _FPOS_T_DEFINED
#define _FPOS_T_DEFINED
typedef long fpos_t;
#endif

#ifndef _WCHAR_T_DEFINED
#define _WCHAR_T_DEFINED
typedef unsigned short wchar_t;
#endif

typedef unsigned short wint_t;
typedef unsigned short wctype_t;

struct _exception {
    int type;       // Exception type - see below
    char *name;     // Name of function where error occured
    double arg1;    // First argument to function
    double arg2;    // Second argument (if any) to function
    double retval;  // Value to be returned by function
};

typedef struct {
    int newmode;
} _startupinfo;

struct _stat {
    unsigned int st_dev;
    unsigned short st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    unsigned int st_rdev;
    long st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
};

struct _stati64 {
    unsigned int st_dev;
    unsigned short st_ino;
    unsigned short st_mode;
    short st_nlink;
    short st_uid;
    short st_gid;
    unsigned int st_rdev;
    __int64 st_size;
    time_t st_atime;
    time_t st_mtime;
    time_t st_ctime;
};

struct _iobuf {
    char *ptr;
    int cnt;
    char *base;
    int flag;
    handle_t file;
    int charbuf;
    int bufsiz;
    char *tmpfname;
};

typedef struct _iobuf FILE;

struct timeb {
    time_t time;
    unsigned short millitm;
    short timezone;
    short dstflag;
};

typedef void (__cdecl *_PVFV)(void);

typedef int (__cdecl *_onexit_t)(void);

#define EOF (-1)

void *memset(void *p, int c, size_t n);

void *memcpy(void *, const void *, size_t);

char *strcpy(char *, const char *);

size_t strlen(const char *);

int strcmp(const char *s1, const char *s2);

int vsprintf(char *buffer, const char *fmt, va_list args);

int atoi(const char *string);

int convert_filename_to_unicode(const char *src, wchar_t *dst, int maxlen);

int convert_filename_from_unicode(const wchar_t *src, char *dst, int maxlen);

#endif
