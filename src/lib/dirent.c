//
// List directory entries
//
#include <os.h>
#include <string.h>
#include <dirent.h>

DIR *opendir(const char *name) {
    int handle;
    DIR *dirp;

    handle = _opendir(name);
    if (handle < 0) return NULL;

    dirp = (DIR *) malloc(sizeof(DIR));
    if (!dirp) {
        close(handle);
        return NULL;
    }
    memset(dirp, 0, sizeof(DIR));
    dirp->handle = handle;
    strcpy(dirp->path, name);
    return dirp;
}

int closedir(DIR *dirp) {
    int rc;

    if (!dirp) {
        errno = EINVAL;
        return -1;
    }

    rc = close(dirp->handle);
    free(dirp);

    return rc;
}

struct dirent *readdir(DIR *dirp) {
    struct direntry dirent;
    int rc;

    if (!dirp) {
        errno = EINVAL;
        return NULL;
    }

    rc = _readdir(dirp->handle, &dirent, 1);
    if (rc <= 0) return NULL;

    dirp->entry.d_ino = dirent.ino;
    dirp->entry.d_namlen = dirent.namelen;
    memcpy(dirp->entry.d_name, dirent.name, dirent.namelen);
    dirp->entry.d_name[dirent.namelen] = 0;

    return &dirp->entry;
}

int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result) {
    struct direntry dirent;
    int rc;

    if (!dirp || !entry) {
        errno = EINVAL;
        return -1;
    }

    rc = _readdir(dirp->handle, &dirent, 1);
    if (rc <= 0) return -1;

    entry->d_ino = dirent.ino;
    entry->d_namlen = dirent.namelen;
    memcpy(entry->d_name, dirent.name, dirent.namelen);
    entry->d_name[dirent.namelen] = 0;

    *result = entry;
    return 0;
}

int rewinddir(DIR *dirp) {
    if (!dirp) {
        errno = EINVAL;
        return -1;
    }

    close(dirp->handle);
    dirp->handle = _opendir(dirp->path);
    if (dirp->handle < 0) return -1;
    return 0;
}
