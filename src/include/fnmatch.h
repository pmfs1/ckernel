//
// Filename matching
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef FNMATCH_H
#define FNMATCH_H

#include <sys/types.h>

#define FNM_NOMATCH      1       // Match failed

#define FNM_NOESCAPE     0x01    // Disable backslash escaping
#define FNM_PATHNAME     0x02    // Slash must be matched by slash
#define FNM_PERIOD       0x04    // Period must be matched by period
#define FNM_CASEFOLD     0x08    // Case-insensitive matching (NYI)
#define FNM_NOSYS        0x00    // Reserved

#ifdef  __cplusplus
extern "C" {
#endif

int fnmatch(const char *pattern, const char *string, int flags);

#ifdef  __cplusplus
}
#endif

#endif
