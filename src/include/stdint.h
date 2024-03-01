//
// Defines integer types
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef STDINT_H
#define STDINT_H

//
// Integer types
//

#ifndef _INT_T_DEFINED
#define _INT_T_DEFINED
typedef signed char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef __int64 int64_t;

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned __int64
uint64_t;
#endif

//
// Types for void * pointers
//

#ifndef _INTPTR_T_DEFINED
#define _INTPTR_T_DEFINED
typedef long intptr_t;
typedef unsigned long uintptr_t;
#endif

//
// Largest integral types
//

#ifndef _INTMAX_T_DEFINED
#define _INTMAX_T_DEFINED
typedef __int64 intmax_t;
typedef unsigned __int64
uintmax_t;
#endif

//
// Limits of integral types
//

#define INT8_MIN     (-128)
#define INT16_MIN    (-32767 - 1)
#define INT32_MIN    (-2147483647 - 1)
#define INT64_MIN    (-9223372036854775807i64 - 1)

#define INT8_MAX     (127)
#define INT16_MAX    (32767)
#define INT32_MAX    (2147483647)
#define INT64_MAX    (9223372036854775807i64)

#define UINT8_MAX    (0xffui8)
#define UINT16_MAX   (0xffffui16)
#define UINT32_MAX   (0xffffffffui32)
#define UINT64_MAX   (0xffffffffffffffffui64)

#define INTMAX_MIN   INT64_MIN
#define INTMAX_MAX   INT64_MAX
#define UINTMAX_MAX  UINT64_MAX

#define SIZE_MAX     UINT32_MAX

#endif
