//
// POSIX semaphore library
//
#include <os.h>
#include <semaphore.h>

int sem_init(sem_t *sem, int pshared, unsigned int value) {
    if (pshared) {
        errno = EPERM;
        return -1;
    }

    *sem = mksem(value);
    if (*sem < 0) {
        errno = ENOSPC;
        return -1;
    }

    return 0;
}

int sem_destroy(sem_t *sem) {
    if (!sem || *sem == -1) {
        errno = EINVAL;
        return -1;
    }

    if (close(*sem) < 0) {
        errno = EINVAL;
        return -1;
    }

    *sem = -1;
    return 0;
}

int sem_trywait(sem_t *sem) {
    if (!sem || *sem == -1) {
        errno = EINVAL;
        return -1;
    }

    if (waitone(*sem, 0) < 0) {
        errno = EAGAIN;
        return -1;
    }

    return 0;
}

int sem_wait(sem_t *sem) {
    if (!sem || *sem == -1) {
        errno = EINVAL;
        return -1;
    }

    if (waitone(*sem, INFINITE) < 0) return -1;
    return 0;
}

int sem_timedwait(sem_t *sem, const struct timespec *abstime) {
    struct timeval curtime;
    long timeout;

    if (!sem || *sem == -1) {
        errno = EINVAL;
        return -1;
    }

    if (gettimeofday(&curtime, NULL) < 0) return -1;
    timeout = ((long) (abstime->tv_sec - curtime.tv_sec) * 1000L +
               (long) ((abstime->tv_nsec / 1000) - curtime.tv_usec) / 1000L);
    if (timeout < 0) timeout = 0L;

    if (waitone(*sem, timeout) < 0) return -1;
    return 0;
}

int sem_post(sem_t *sem) {
    if (!sem || *sem == -1) {
        errno = EINVAL;
        return -1;
    }

    if (semrel(*sem, 1) < 0) {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

int sem_post_multiple(sem_t *sem, int count) {
    if (!sem || *sem == -1) {
        errno = EINVAL;
        return -1;
    }

    if (semrel(*sem, count) < 0) {
        errno = EINVAL;
        return -1;
    }

    return 0;
}

int sem_open(const char *name, int oflag, int mode, unsigned int value) {
    errno = ENOSYS;
    return -1;
}

int sem_close(sem_t *sem) {
    errno = ENOSYS;
    return -1;
}

int sem_unlink(const char *name) {
    errno = ENOSYS;
    return -1;
}

int sem_getvalue(sem_t *sem, int *sval) {
    errno = ENOSYS;
    return -1;
}
