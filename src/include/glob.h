//
// Path name pattern matching
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef GLOB_H
#define GLOB_H

#include <sys/types.h>

typedef struct {
    size_t gl_pathc;    // Count of total paths so far
    size_t gl_matchc;   // Count of paths matching pattern
    size_t gl_offs;     // Reserved at beginning of gl_pathv
    int gl_flags;       // Copy of flags parameter to glob
    char **gl_pathv;    // List of paths matching pattern

    // Copy of errfunc parameter to glob
    int (*gl_errfunc)(const char *, int);
} glob_t;

#define GLOB_APPEND   0x0001  // Append to output from previous call
#define GLOB_DOOFFS   0x0002  // Specify how many null pointers to add to the beginning of gl_pathv
#define GLOB_ERR      0x0004  // Return on error
#define GLOB_MARK     0x0008  // Append / to matching directories
#define GLOB_NOCHECK  0x0010  // Return pattern itself if nothing matches
#define GLOB_NOSORT   0x0020  // Don't sort
#define GLOB_NOESCAPE 0x2000  // Disable backslash escaping

#define GLOB_BRACE    0x0080  // Expand braces ala csh
#define GLOB_MAGCHAR  0x0100  // Pattern had globbing characters
#define GLOB_NOMAGIC  0x0200  // GLOB_NOCHECK without magic chars (csh)
#define GLOB_QUOTE    0x0400  // Quote special chars with \ 
#define GLOB_TILDE    0x0800  // Expand tilde names from the passwd file
#define GLOB_LIMIT    0x1000  // Limit number of returned paths

// Source compatibility, these are the old names
#define GLOB_MAXPATH   GLOB_LIMIT
#define GLOB_ABEND     GLOB_ABORTED

//
// Error values returned by glob(3)
//

#define GLOB_NOSPACE  (-1)  // Malloc call failed
#define GLOB_ABORTED  (-2)  // Unignored error
#define GLOB_NOMATCH  (-3)  // No match and GLOB_NOCHECK was not set
#define GLOB_NOSYS    (-4)  // Obsolete: source comptability only

#ifdef  __cplusplus
extern "C" {
#endif

int glob(const char *pattern, int flags, int (*errfunc)(const char *, int), glob_t *pglob);

void globfree(glob_t *pglob);

#ifdef  __cplusplus
}
#endif

#endif
