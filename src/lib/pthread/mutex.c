//
// POSIX mutexes
//
#include <os.h>
#include <pthread.h>
#include <atomic.h>

int pthread_mutexattr_init(pthread_mutexattr_t *attr) {
    if (!attr) return EINVAL;
    attr->pshared = PTHREAD_PROCESS_PRIVATE;
    attr->kind = PTHREAD_MUTEX_DEFAULT;
    return 0;
}

int pthread_mutexattr_destroy(pthread_mutexattr_t *attr) {
    return 0;
}

int pthread_mutexattr_getpshared(const pthread_mutexattr_t *attr, int *pshared) {
    if (!attr || !pshared) return EINVAL;
    *pshared = attr->pshared;
    return 0;
}

int pthread_mutexattr_setpshared(pthread_mutexattr_t *attr, int pshared) {
    if (!attr) return EINVAL;
    if (pshared != PTHREAD_PROCESS_PRIVATE && pshared != PTHREAD_PROCESS_SHARED) return EINVAL;
    attr->pshared = pshared;
    return 0;
}

int pthread_mutexattr_gettype(pthread_mutexattr_t *attr, int *kind) {
    if (!attr || !kind) return EINVAL;
    *kind = attr->kind;
    return 0;
}

int pthread_mutexattr_settype(pthread_mutexattr_t *attr, int kind) {
    if (!attr) return EINVAL;
    if (kind != PTHREAD_MUTEX_NORMAL &&
        kind != PTHREAD_MUTEX_RECURSIVE &&
        kind != PTHREAD_MUTEX_ERRORCHECK) {
        return EINVAL;
    }
    attr->kind = kind;
    return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr) {
    if (!mutex) return EINVAL;
    if (attr && attr->pshared == PTHREAD_PROCESS_SHARED) return ENOSYS;

    mutex->lock = 0;
    mutex->recursion = 0;
    mutex->kind = attr ? attr->kind : PTHREAD_MUTEX_DEFAULT;
    mutex->owner = NOHANDLE;
    mutex->event = mkevent(0, 0);
    if (mutex->event < 0) return ENOSPC;
    return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex) {
    if (!mutex) return EINVAL;
    if (close(mutex->event) < 0) return errno;
    return 0;
}

int pthread_mutex_lock(pthread_mutex_t *mutex) {
    if (mutex->kind == PTHREAD_MUTEX_NORMAL) {
        if (atomic_exchange(&mutex->lock, 1) != 0) {
            while (atomic_exchange(&mutex->lock, -1) != 0) {
                if (waitone(mutex->event, INFINITE) != 0) return EINVAL;
            }
        }
    } else {
        pthread_t self = pthread_self();

        if (atomic_exchange(&mutex->lock, 1) == 0) {
            mutex->recursion = 1;
            mutex->owner = self;
        } else {
            if (pthread_equal(mutex->owner, self)) {
                if (mutex->kind == PTHREAD_MUTEX_RECURSIVE) {
                    mutex->recursion++;
                } else {
                    return EDEADLK;
                }
            } else {
                while (atomic_exchange(&mutex->lock, -1) != 0) {
                    if (waitone(mutex->event, INFINITE) != 0) return EINVAL;
                    mutex->recursion = 1;
                    mutex->owner = self;
                }
            }
        }
    }

    return 0;
}

int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abstime) {
    if (mutex->kind == PTHREAD_MUTEX_NORMAL) {
        if (atomic_exchange(&mutex->lock, 1) != 0) {
            while (atomic_exchange(&mutex->lock, -1) != 0) {
                if (waitone(mutex->event, __abstime2timeout(abstime)) != 0) return EINVAL;
            }
        }
    } else {
        pthread_t self = pthread_self();

        if (atomic_exchange(&mutex->lock, 1) == 0) {
            mutex->recursion = 1;
            mutex->owner = self;
        } else {
            if (pthread_equal(mutex->owner, self)) {
                if (mutex->kind == PTHREAD_MUTEX_RECURSIVE) {
                    mutex->recursion++;
                } else {
                    return EDEADLK;
                }
            } else {
                while (atomic_exchange(&mutex->lock, -1) != 0) {
                    if (waitone(mutex->event, __abstime2timeout(abstime)) != 0) return EINVAL;
                    mutex->recursion = 1;
                    mutex->owner = self;
                }
            }
        }
    }

    return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex) {
    if (atomic_compare_and_exchange(&mutex->lock, 1, 0) == 0) {
        if (mutex->kind != PTHREAD_MUTEX_NORMAL) {
            mutex->recursion = 1;
            mutex->owner = pthread_self();
        }
    } else {
        if (mutex->kind == PTHREAD_MUTEX_RECURSIVE && pthread_equal(mutex->owner, pthread_self())) {
            mutex->recursion++;
        } else {
            return EBUSY;
        }
    }

    return 0;
}

int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    if (mutex->kind == PTHREAD_MUTEX_NORMAL) {
        long idx;

        idx = atomic_exchange(&mutex->lock, 0);
        if (idx != 0) {
            if (idx < 0) {
                if (eset(mutex->event) < 0) return EINVAL;
            }
        } else {
            return EPERM;
        }
    } else {
        if (pthread_equal(mutex->owner, pthread_self())) {
            if (mutex->kind != PTHREAD_MUTEX_RECURSIVE || --mutex->recursion == 0) {
                mutex->owner = NOHANDLE;
                if (atomic_exchange(&mutex->lock, 0) < 0) {
                    if (eset(mutex->event) < 0) return EINVAL;
                }
            }
        } else {
            return EPERM;
        }
    }

    return 0;
}
