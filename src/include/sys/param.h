//
// System parameters
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_PARAM_H
#define SYS_PARAM_H

#include <sys/types.h>
#include <limits.h>

#define BIG_ENDIAN      4321
#define LITTLE_ENDIAN   1234
#define BYTE_ORDER      LITTLE_ENDIAN

#define MAXPATHLEN PATH_MAX

#endif
