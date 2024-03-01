//
// User management
//
#include <os/krnl.h>

int getuid() {
    return self()->ruid;
}

int setuid(uid_t uid) {
    struct thread *thread = self();

    if (thread->euid == 0) {
        thread->ruid = thread->euid = uid;
    } else if (uid == thread->ruid) {
        thread->euid = uid;
    } else {
        return -EPERM;
    }

    return 0;
}

int getgid() {
    return self()->rgid;
}

int setgid(gid_t gid) {
    struct thread *thread = self();

    if (thread->euid == 0) {
        thread->rgid = thread->egid = gid;
    } else if (gid == thread->rgid) {
        thread->egid = gid;
    } else {
        return -EPERM;
    }

    return 0;
}

int geteuid() {
    return self()->euid;
}

int seteuid(uid_t uid) {
    struct thread *thread = self();

    if (thread->euid == 0 || uid == thread->ruid) {
        thread->euid = uid;
    } else {
        return -EPERM;
    }

    return 0;
}

int getegid() {
    return self()->egid;
}

int setegid(gid_t gid) {
    struct thread *thread = self();

    if (thread->euid == 0 || gid == thread->rgid) {
        thread->egid = gid;
    } else {
        return -EPERM;
    }

    return 0;
}

int getgroups(int size, gid_t *list) {
    struct thread *thread = self();

    if (!list || size < thread->ngroups) return -EINVAL;
    memcpy(list, thread->groups, thread->ngroups * sizeof(gid_t));
    return thread->ngroups;
}

int setgroups(int size, gid_t *list) {
    struct thread *thread = self();

    if (thread->euid != 0) return -EPERM;
    if (!list || size < 0 || size > NGROUPS_MAX) return -EINVAL;
    thread->ngroups = size;
    memcpy(thread->groups, list, size * sizeof(gid_t));

    return 0;
}

int check(int mode, uid_t uid, gid_t gid, int access) {
    struct thread *thread = self();

    if (thread->euid == 0) return 0;

    if (thread->euid != uid) {
        access >>= 3;
        if (thread->egid != gid) access >>= 3;
    }

    if ((mode & access) == access) return 0;

    return -EACCES;
}
