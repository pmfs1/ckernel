//
// Memory-mapped files
//
#if _MSC_VER > 1000
#pragma once
#endif

#ifndef SYS_MMAN_H
#define SYS_MMAN_H

#include <sys/types.h>

#define MAP_FAILED NULL

//
// Protection options
//

#define PROT_NONE     0     // Page cannot be accessed
#define PROT_READ     1     // Page can be read
#define PROT_WRITE    2     // Page can be written
#define PROT_EXEC     4     // Page can be executed

//
// Sharing flags
//

#define MAP_PRIVATE   1     // Changes are private
#define MAP_SHARED    2     // Share changes
#define MAP_FIXED     4     // Allocate at fixed address
#define MAP_ANONYMOUS 8     // Anonymous mapping that is not backed by any file
#define MAP_FILE      16    // File mapping

#define MAP_ANON      MAP_ANONYMOUS

//
// Synchronization flags
//

#define MS_ASYNC       1     // Perform asynchronous writes
#define MS_SYNC        2     // Perform synchronous writes
#define MS_INVALIDATE  4     // Invalidate mappings

//
// Process memory locking options
//

#define MCL_CURRENT    1     // Lock currently mapped pages
#define MCL_FUTURE     2     // Lock pages that become mapped

#ifdef  __cplusplus
extern "C" {
#endif

void *mmap(void *addr, size_t size, int prot, int flags, handle_t f, off_t offset);

int munmap(void *addr, size_t size);

int msync(void *addr, size_t size, int flags);

int mprotect(void *addr, size_t size, int prot);

int mlock(const void *addr, size_t size);

int munlock(const void *addr, size_t size);

int mlockall(int flags);

int munlockall(void);

#ifdef  __cplusplus
}
#endif

#endif
