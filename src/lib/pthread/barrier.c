//
// POSIX barriers
//
#include <os.h>
#include <pthread.h>
#include <atomic.h>

int pthread_barrierattr_init(pthread_barrierattr_t *attr) {
    if (!attr) return EINVAL;
    attr->pshared = PTHREAD_PROCESS_PRIVATE;
    return 0;
}

int pthread_barrierattr_destroy(pthread_barrierattr_t *attr) {
    return 0;
}

int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared) {
    if (!attr || !pshared) return EINVAL;
    *pshared = attr->pshared;
    return 0;
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared) {
    if (!attr) return EINVAL;
    if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED) return EINVAL;
    attr->pshared = pshared;
    return 0;
}

int pthread_barrier_init(pthread_barrier_t *barrier, const pthread_barrierattr_t *attr, unsigned int count) {
    if (!barrier || count == 0) return EINVAL;
    if (attr && attr->pshared == PTHREAD_PROCESS_SHARED) return ENOSYS;

    barrier->curr_height = barrier->init_height = count;
    barrier->step = 0;

    barrier->breeched[0] = mksem(0);
    if (barrier->breeched[0] < 0) return errno;

    barrier->breeched[1] = mksem(0);
    if (barrier->breeched[1] < 0) {
        int rc = errno;
        close(barrier->breeched[0]);
        return rc;
    }

    return 0;
}

int pthread_barrier_destroy(pthread_barrier_t *barrier) {
    if (!barrier) return EINVAL;
    if (close(barrier->breeched[0]) < 0) return errno;
    if (close(barrier->breeched[1]) < 0) return errno;
    return 0;
}

int pthread_barrier_wait(pthread_barrier_t *barrier) {
    int step = barrier->step;
    int rc;

    if (atomic_decrement(&barrier->curr_height) == 0) {
        barrier->curr_height = barrier->init_height;
        if (barrier->init_height > 1) {
            rc = semrel(barrier->breeched[step], barrier->init_height - 1);
        } else {
            rc = 0;
        }
    } else {
        rc = waitone(barrier->breeched[step], INFINITE);
    }

    if (rc == 0) {
        if (atomic_compare_and_exchange(&barrier->step, 1 - step, step) == step) {
            rc = PTHREAD_BARRIER_SERIAL_THREAD;
        } else {
            rc = 0;
        }
    }

    return rc;
}
