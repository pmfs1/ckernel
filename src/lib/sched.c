//
// POSIX scheduling library
//
#include <os.h>
#include <sched.h>

int sched_yield(void) {
    msleep(0);
    return 0;
}

int sched_get_priority_min(int policy) {
    if (policy < SCHED_MIN || policy > SCHED_MAX) {
        errno = EINVAL;
        return -1;
    }

    return PRIORITY_IDLE;
}

int sched_get_priority_max(int policy) {
    if (policy < SCHED_MIN || policy > SCHED_MAX) {
        errno = EINVAL;
        return -1;
    }

    return PRIORITY_TIME_CRITICAL;
}

int sched_setscheduler(pid_t pid, int policy) {
    if (policy != SCHED_OTHER) {
        errno = ENOSYS;
        return -1;
    }

    return SCHED_OTHER;
}

int sched_getscheduler(pid_t pid) {
    return SCHED_OTHER;
}
