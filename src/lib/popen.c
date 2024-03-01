//
// Pipe I/O
//
#include <os.h>

#include <stdio.h>
#include <string.h>

#define SHELL "sh.exe"

FILE *popen(const char *command, const char *mode) {
    char *cmdline;
    int cmdlen;
    int rc;
    int hndl[2];
    int phndl;
    struct tib *tib;
    struct process *proc;
    FILE *f;

    if (!command || *mode != 'r' && *mode != 'w') {
        errno = EINVAL;
        return NULL;
    }

    cmdlen = strlen(SHELL) + 1 + strlen(command);
    cmdline = malloc(cmdlen + 1);
    if (!cmdline) {
        errno = ENOMEM;
        return NULL;
    }
    strcpy(cmdline, SHELL);
    strcat(cmdline, " ");
    strcat(cmdline, command);

    phndl = spawn(P_SUSPEND, SHELL, cmdline, NULL, &tib);
    free(cmdline);
    if (phndl < 0) return NULL;
    proc = tib->proc;

    rc = pipe(hndl);
    if (rc < 0) return NULL;

    if (*mode == 'w') {
        if (proc->iob[0] != NOHANDLE) close(proc->iob[0]);
        proc->iob[0] = hndl[0];
        f = fdopen(hndl[1], mode);
    } else {
        if (proc->iob[1] != NOHANDLE) close(proc->iob[1]);
        proc->iob[1] = hndl[1];
        if (mode[1] == '2') dup2(hndl[1], proc->iob[2]);
        f = fdopen(hndl[0], mode);
    }

    if (f == NULL) {
        close(phndl);
        return NULL;
    }

    f->phndl = phndl;
    resume(phndl);
    return f;
}

int pclose(FILE *stream) {
    int rc;

    if (stream->flag & _IORD) {
        waitone(stream->phndl, INFINITE);
        close(stream->phndl);
        rc = fclose(stream);
    } else {
        int phndl = stream->phndl;
        rc = fclose(stream);
        waitone(phndl, INFINITE);
        close(phndl);
    }

    return rc;
}
