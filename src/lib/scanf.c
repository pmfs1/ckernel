//
// Formatted input
//
#include <stdio.h>
#include <stdarg.h>

int _input(FILE *stream, const unsigned char *format, va_list arglist);

int fscanf(FILE *stream, const char *fmt, ...) {
    int rc;
    va_list args;

    va_start(args, fmt);

    rc = _input(stream, fmt, args);

    return rc;
}

int vfscanf(FILE *stream, const char *fmt, va_list args) {
    return _input(stream, fmt, args);
}

int scanf(const char *fmt, ...) {
    int rc;
    va_list args;

    va_start(args, fmt);

    rc = _input(stdin, fmt, args);

    return rc;
}

int vscanf(const char *fmt, va_list args) {
    return _input(stdin, fmt, args);
}

int sscanf(const char *buffer, const char *fmt, ...) {
    int rc;
    va_list args;
    FILE str;

    va_start(args, fmt);

    str.flag = _IORD | _IOSTR | _IOOWNBUF;
    str.ptr = str.base = (char *) buffer;
    str.cnt = strlen(buffer);
    rc = _input(&str, fmt, args);

    return rc;
}

int vsscanf(const char *buffer, const char *fmt, va_list args) {
    int rc;
    FILE str;

    str.flag = _IORD | _IOSTR | _IOOWNBUF;
    str.ptr = str.base = (char *) buffer;
    str.cnt = strlen(buffer);
    rc = _input(&str, fmt, args);

    return rc;
}
