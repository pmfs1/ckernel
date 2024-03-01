//
// String to number conversion
//
#include <os.h>
#include <ctype.h>
#include <stdint.h>

#define FL_UNSIGNED   1
#define FL_NEG        2
#define FL_OVERFLOW   4
#define FL_READDIGIT  8

static unsigned long strtoxl(const char *nptr, char **endptr, int ibase, int flags) {
    const unsigned char *p;
    char c;
    unsigned long number;
    unsigned digval;
    unsigned long maxval;

    p = (const unsigned char *) nptr;
    number = 0;

    c = *p++;
    while (isspace(c)) c = *p++;

    if (c == '-') {
        flags |= FL_NEG;
        c = *p++;
    } else if (c == '+') {
        c = *p++;
    }

    if (ibase < 0 || ibase == 1 || ibase > 36) {
        if (endptr) *endptr = (char *) nptr;
        return 0L;
    } else if (ibase == 0) {
        if (c != '0') {
            ibase = 10;
        } else if (*p == 'x' || *p == 'X') {
            ibase = 16;
        } else {
            ibase = 8;
        }
    }

    if (ibase == 16) {
        if (c == '0' && (*p == 'x' || *p == 'X')) {
            ++p;
            c = *p++;
        }
    }

    maxval = UINT32_MAX / ibase;

    for (;;) {
        if (isdigit(c)) {
            digval = c - '0';
        } else if (isalpha(c)) {
            digval = toupper(c) - 'A' + 10;
        } else {
            break;
        }

        if (digval >= (unsigned) ibase) break;

        flags |= FL_READDIGIT;

        if (number < maxval || (number == maxval && (unsigned long) digval <= UINT32_MAX % ibase)) {
            number = number * ibase + digval;
        } else {
            flags |= FL_OVERFLOW;
        }

        c = *p++;
    }

    --p;

    if (!(flags & FL_READDIGIT)) {
        if (endptr) p = nptr;
        number = 0;
    } else if ((flags & FL_OVERFLOW) || (!(flags & FL_UNSIGNED) && (((flags & FL_NEG) && (number < INT32_MIN)) ||
                                                                    (!(flags & FL_NEG) && (number > INT32_MAX))))) {
#ifndef KERNEL
        errno = ERANGE;
#endif

        if (flags & FL_UNSIGNED) {
            number = UINT32_MAX;
        } else if (flags & FL_NEG) {
            number = INT32_MIN;
        } else {
            number = INT32_MAX;
        }
    }

    if (endptr) *endptr = (char *) p;

    if (flags & FL_NEG) number = (unsigned long) (-(long) number);

    return number;
}

long strtol(const char *nptr, char **endptr, int ibase) {
    return (long) strtoxl(nptr, endptr, ibase, 0);
}

unsigned long strtoul(const char *nptr, char **endptr, int ibase) {
    return strtoxl(nptr, endptr, ibase, FL_UNSIGNED);
}

long atol(const char *nptr) {
    int c;
    int sign;
    long total;
    const unsigned char *p = (const unsigned char *) nptr;

    while (isspace(*p)) ++p;

    c = *p++;
    sign = c;
    if (c == '-' || c == '+') c = *p++;

    total = 0;
    while (isdigit(c)) {
        total = 10 * total + (c - '0');
        c = *p++;
    }

    if (sign == '-') {
        return -total;
    } else {
        return total;
    }
}

int atoi(const char *nptr) {
    return (int) atol(nptr);
}

#ifndef KERNEL
static unsigned __int64

strtoxll(const char *nptr, char **endptr, int ibase, int flags) {
    const unsigned char *p = (const unsigned char *) nptr;
    unsigned __int64
    number = 0;
    unsigned __int64
    digval;
    unsigned __int64
    maxval;
    int c;

    c = *p++;
    while (isspace(c)) c = *p++;

    if (c == '-') {
        flags |= FL_NEG;
        c = *p++;
    } else if (c == '+') {
        c = *p++;
    }

    if (ibase < 0 || ibase == 1 || ibase > 36) {
        if (endptr) *endptr = (char *) nptr;
        return 0;
    } else if (ibase == 0) {
        if (c != '0') {
            ibase = 10;
        } else if (*p == 'x' || *p == 'X') {
            ibase = 16;
        } else {
            ibase = 8;
        }
    }

    if (ibase == 16) {
        if (c == '0' && (*p == 'x' || *p == 'X')) {
            ++p;
            c = *p++;
        }
    }

    maxval = INT64_MAX / ibase;

    for (;;) {
        if (isdigit(c)) {
            digval = c - '0';
        } else if (isalpha(c)) {
            digval = toupper(c) - 'A' + 10;
        } else {
            break;
        }

        if (digval >= (unsigned __int64) ibase) break;

        flags |= FL_READDIGIT;

        if (number < maxval || (number == maxval && digval <=(unsigned __int64) (INT64_MAX % ibase))) {
            number = number * ibase + digval;
        } else {
            flags |= FL_OVERFLOW;
        }

        c = *p++;
    }

    --p;

    if (!(flags & FL_READDIGIT)) {
        if (endptr) p = nptr;
        number = 0;
    } else if ((flags & FL_OVERFLOW) || (!(flags & FL_UNSIGNED) && (((flags & FL_NEG) && (number < INT64_MIN)) ||
                                                                    (!(flags & FL_NEG) && (number > INT64_MAX))))) {
        errno = ERANGE;

        if (flags & FL_UNSIGNED) {
            number = UINT64_MAX;
        } else if (flags & FL_NEG) {
            number = INT64_MIN;
        } else {
            number = INT64_MAX;
        }
    }

    if (endptr) *endptr = (char *) p;
    if (flags & FL_NEG) number = (unsigned __int64) (-(__int64)
    number);

    return number;
}

__int64 strtoll(const char *nptr, char **endptr, int ibase) {
    return (__int64)
    strtoxll(nptr, endptr, ibase, 0);
}

unsigned __int64

strtoull(const char *nptr, char **endptr, int ibase) {
    return strtoxll(nptr, endptr, ibase, FL_UNSIGNED);
}

intmax_t strtoimax(const char *nptr, char **endptr, int ibase) {
    return (intmax_t) strtoxll(nptr, endptr, ibase, 0);
}

uintmax_t strtoumax(const char *nptr, char **endptr, int ibase) {
    return strtoxll(nptr, endptr, ibase, FL_UNSIGNED);
}

#endif
