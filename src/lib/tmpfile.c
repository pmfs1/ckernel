//
// Temporary files
//
#include <os.h>

#include <stdio.h>
#include <string.h>

static int gentmpfn(char *path, char *prefix, int unique, char *tempfn) {
    const char *format = "%s%c%s%4.4x.tmp";
    int len;

    len = strlen(path);
    if (len > 0 && (path[len - 1] == PS1 || path[len - 1] == PS2)) len--;

    if (unique == 0) unique = clock();

    sprintf(tempfn, format, path, PS1, prefix, unique);
    while (access(tempfn, 0) == 0) {
        unique++;
        sprintf(tempfn, format, path, PS1, prefix, unique);
    }
    if (errno != ENOENT) return -1;

    return unique;
}

FILE *tmpfile() {
    static int unique = 0;

    FILE *stream;
    char *path;
    char tempfn[MAXPATH];
    int rc;

    path = getenv("tmp");
    if (!path) path = "/tmp";

    while (1) {
        rc = gentmpfn(path, "t", unique, tempfn);
        if (rc < 0) return NULL;
        unique = rc;

        stream = fopen(tempfn, "wb+TD");
        if (stream != NULL) break;
        if (errno != EEXIST) return NULL;
    }

    return stream;
}

char *tmpnam(char *string) {
    static int unique = 0;
    char *path;
    char *tempfn;
    int rc;

    if (string) {
        tempfn = string;
    } else {
        tempfn = gettib()->tmpnambuf;
    }

    path = getenv("tmp");
    if (!path) path = "/tmp";

    rc = gentmpfn(path, "s", 0, tempfn);
    if (rc < 0) return NULL;
    unique = rc;

    return tempfn;
}

char *tempnam(const char *dir, const char *prefix) {
    static int unique = 0;
    char *path;
    char *tempfn;
    int rc;

    tempfn = gettib()->tmpnambuf;

    path = (char *) dir;
    if (!path) path = getenv("tmp");
    if (!path) path = ".";

    rc = gentmpfn(path, (char *) prefix, unique, tempfn);
    if (rc < 0) return NULL;
    unique = rc;

    return strdup(tempfn);
}
