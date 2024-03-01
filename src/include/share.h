//
// File sharing modes for sopen
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SHARE_H
#define SHARE_H

#ifndef SH_COMPAT
#define SH_COMPAT

#ifndef SH_COMPAT

#define SH_COMPAT               0x0000
#define SH_DENYRW               0x0010  // Denies read and write access to file
#define SH_DENYWR               0x0020  // Denies write access to file
#define SH_DENYRD               0x0030  // Denies read access to file
#define SH_DENYNO               0x0040  // Permits read and write access

#endif

#endif

#endif
