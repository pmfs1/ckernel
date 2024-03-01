//
// Definitions for pattern matching functions
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef LIBGEN_H
#define LIBGEN_H

#ifdef  __cplusplus
extern "C" {
#endif

char *basename(char *path);

char *dirname(char *path);

#ifdef  __cplusplus
}
#endif

#endif
