//
// POSIX spin locks
//
#include <os.h>
#include <pthread.h>
#include <atomic.h>

int pthread_spin_init(pthread_spinlock_t *lock, int pshared) {
    if (!lock) return EINVAL;

    lock->interlock = SPINLOCK_USEMUTEX;
    pthread_mutex_init(&lock->mutex, NULL);

    return 0;
}

int pthread_spin_destroy(pthread_spinlock_t *lock) {
    if (!lock) return EINVAL;
    if (lock->interlock == SPINLOCK_USEMUTEX) pthread_mutex_destroy(&lock->mutex);
    return 0;
}

int pthread_spin_lock(pthread_spinlock_t *lock) {
    while (atomic_compare_and_exchange(&lock->interlock, SPINLOCK_LOCKED, SPINLOCK_UNLOCKED) == SPINLOCK_LOCKED);

    if (lock->interlock == SPINLOCK_LOCKED) {
        return 0;
    } else if (lock->interlock == SPINLOCK_USEMUTEX) {
        return pthread_mutex_lock(&lock->mutex);
    }

    return EINVAL;
}

int pthread_spin_trylock(pthread_spinlock_t *lock) {
    switch (atomic_compare_and_exchange(&lock->interlock, SPINLOCK_LOCKED, SPINLOCK_UNLOCKED)) {
        case SPINLOCK_UNLOCKED:
            return 0;

        case SPINLOCK_LOCKED:
            return EBUSY;

        case SPINLOCK_USEMUTEX:
            return pthread_mutex_trylock(&lock->mutex);
    }

    return EINVAL;
}

int pthread_spin_unlock(pthread_spinlock_t *lock) {
    switch (atomic_compare_and_exchange(&lock->interlock, SPINLOCK_UNLOCKED, SPINLOCK_LOCKED)) {
        case SPINLOCK_LOCKED:
            return 0;

        case SPINLOCK_UNLOCKED:
            return EPERM;

        case SPINLOCK_USEMUTEX:
            return pthread_mutex_unlock(&lock->mutex);
    }

    return EINVAL;
}
