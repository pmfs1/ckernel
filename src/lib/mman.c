//
// Memory-mapped files
//
#include <os.h>
#include <sys/mman.h>

static int map_protect(int prot) {
    switch (prot) {
        case PROT_NONE:
            return PAGE_NOACCESS;

        case PROT_READ:
            return PAGE_READONLY;

        case PROT_WRITE:
        case PROT_READ | PROT_WRITE:
            return PAGE_READWRITE;

        case PROT_EXEC:
            return PAGE_EXECUTE;

        case PROT_EXEC | PROT_READ:
            return PAGE_EXECUTE_READ;

        case PROT_EXEC | PROT_WRITE:
        case PROT_EXEC | PROT_READ | PROT_WRITE:
            return PAGE_EXECUTE_READWRITE;
    }

    return -1;
}

void *mmap(void *addr, size_t size, int prot, int flags, handle_t f, off_t offset) {
    if (flags & MAP_ANONYMOUS) {
        addr = vmalloc(addr, size, MEM_RESERVE | MEM_COMMIT, map_protect(prot), 'MMAP');
    } else {
        addr = vmmap(addr, size, map_protect(prot), f, offset);
        if (!addr) return MAP_FAILED;
    }

    return addr;
}

int munmap(void *addr, size_t size) {
    return vmfree(addr, size, MEM_DECOMMIT | MEM_RELEASE);
}

int msync(void *addr, size_t size, int flags) {
    return vmsync(addr, size);
}

int mprotect(void *addr, size_t size, int prot) {
    return vmprotect(addr, size, map_protect(prot));
}

int mlock(const void *addr, size_t size) {
    return vmlock((void *) addr, size);
}

int munlock(const void *addr, size_t size) {
    return vmunlock((void *) addr, size);
}

int mlockall(int flags) {
    errno = ENOSYS;
    return -1;
}

int munlockall(void) {
    errno = ENOSYS;
    return -1;
}
