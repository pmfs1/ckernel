//
// Formatted print
//
#include <stdio.h>
#include <stdarg.h>
#include <limits.h>

int _output(FILE *stream, const char *format, va_list args);

int _stbuf(FILE *stream, char *buf, int bufsiz);

void _ftbuf(FILE *stream);

int vfprintf(FILE *stream, const char *fmt, va_list args) {
    int rc;

    if (stream->flag & _IONBF) {
        char buf[BUFSIZ];

        _stbuf(stream, buf, BUFSIZ);
        rc = _output(stream, fmt, args);
        _ftbuf(stream);
    } else {
        rc = _output(stream, fmt, args);
    }

    return rc;
}

int fprintf(FILE *stream, const char *fmt, ...) {
    int rc;
    va_list args;

    va_start(args, fmt);

    if (stream->flag & _IONBF) {
        char buf[BUFSIZ];

        _stbuf(stream, buf, BUFSIZ);
        rc = _output(stream, fmt, args);
        _ftbuf(stream);
    } else {
        rc = _output(stream, fmt, args);
    }

    return rc;
}

int vprintf(const char *fmt, va_list args) {
    return vfprintf(stdout, fmt, args);
}

int printf(const char *fmt, ...) {
    va_list args;

    va_start(args, fmt);
    return vfprintf(stdout, fmt, args);
}

int vsprintf(char *buf, const char *fmt, va_list args) {
    FILE str;
    int rc;

    str.cnt = INT_MAX;
    str.flag = _IOWR | _IOSTR;
    str.ptr = str.base = buf;

    rc = _output(&str, fmt, args);
    if (buf != NULL) putc('\0', &str);

    return rc;
}

int sprintf(char *buf, const char *fmt, ...) {
    va_list args;
    FILE str;
    int rc;

    va_start(args, fmt);

    str.cnt = INT_MAX;
    str.flag = _IOWR | _IOSTR;
    str.ptr = str.base = buf;

    rc = _output(&str, fmt, args);
    if (buf != NULL) putc('\0', &str);

    return rc;
}

int vsnprintf(char *buf, size_t count, const char *fmt, va_list args) {
    FILE str;
    int rc;

    str.cnt = (int) count;
    str.flag = _IOWR | _IOSTR;
    str.ptr = str.base = buf;

    rc = _output(&str, fmt, args);
    if (buf != NULL) putc('\0', &str);

    return rc;
}

int snprintf(char *buf, size_t count, const char *fmt, ...) {
    va_list args;
    FILE str;
    int rc;

    va_start(args, fmt);

    str.cnt = (int) count;
    str.flag = _IOWR | _IOSTR;
    str.ptr = str.base = buf;

    rc = _output(&str, fmt, args);
    if (buf != NULL) putc('\0', &str);

    return rc;
}
