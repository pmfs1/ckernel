//
// Critical sections
//
#include <os.h>
#include <atomic.h>

__inline tid_t threadid() {
    struct tib *tib;

    __asm {
            mov eax, fs:[TIB_SELF_OFFSET]
            mov[tib], eax
    }

    return tib->tid;
}

void mkcs(critsect_t cs) {
    cs->count = -1;
    cs->recursion = 0;
    cs->owner = NOHANDLE;
    cs->event = mkevent(0, 0);
}

void csfree(critsect_t cs) {
    close(cs->event);
}

void enter(critsect_t cs) {
    tid_t tid = threadid();

    if (cs->owner == tid) {
        cs->recursion++;
    } else {
        if (atomic_add(&cs->count, 1) > 0) while (waitone(cs->event, INFINITE) != 0);
        cs->owner = tid;
    }
}

void leave(critsect_t cs) {
    if (cs->owner != threadid()) return;
    if (cs->recursion > 0) {
        cs->recursion--;
    } else {
        cs->owner = NOHANDLE;
        if (atomic_add(&cs->count, -1) >= 0) eset(cs->event);
    }
}
