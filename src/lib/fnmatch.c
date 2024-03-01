//
// Filename matching
//
#include <fnmatch.h>
#include <string.h>

#define  EOS '\0'

static const char *rangematch(const char *pattern, int test, int flags);

//
// fnmatch()
//  Compares filename/pathname to a pattern
//
// As specified in POSIX 1003.2-1992, section B.6.
//

int fnmatch(const char *pattern, const char *string, int flags) {
    const char *stringstart = string;
    char c;
    char test;

    for (;;) {
        switch (c = *pattern++) {
            case EOS:
                return *string == EOS ? 0 : FNM_NOMATCH;

            case '?':
                if (*string == EOS) return FNM_NOMATCH;
                if (*string == '/' && (flags & FNM_PATHNAME)) return FNM_NOMATCH;

                if (*string == '.' &&
                    (flags & FNM_PERIOD) &&
                    (string == stringstart || ((flags & FNM_PATHNAME) && *(string - 1) == '/'))) {
                    return FNM_NOMATCH;
                }

                string++;
                break;

            case '*':
                c = *pattern;
                // Collapse multiple stars
                while (c == '*') c = *++pattern;

                if (*string == '.' &&
                    (flags & FNM_PERIOD) &&
                    (string == stringstart || ((flags & FNM_PATHNAME) && *(string - 1) == '/'))) {
                    return FNM_NOMATCH;
                }

                // Optimize for pattern with * at end or before
                if (c == EOS) {
                    if (flags & FNM_PATHNAME) {
                        return strchr(string, '/') == NULL ? 0 : FNM_NOMATCH;
                    } else {
                        return 0;
                    }
                } else if (c == '/' && flags & FNM_PATHNAME) {
                    if ((string = strchr(string, '/')) == NULL) return FNM_NOMATCH;
                    break;
                }

                // General case, use recursion
                while ((test = *string) != EOS) {
                    if (!fnmatch(pattern, string, flags & ~FNM_PERIOD)) return 0;
                    if (test == '/' && flags & FNM_PATHNAME) break;
                    string++;
                }
                return FNM_NOMATCH;

            case '[':
                if (*string == EOS) return FNM_NOMATCH;
                if (*string == '/' && flags & FNM_PATHNAME) return FNM_NOMATCH;
                if ((pattern = rangematch(pattern, *string, flags)) == NULL) return FNM_NOMATCH;
                string++;
                break;

            case '\\':
                if (!(flags & FNM_NOESCAPE)) {
                    if ((c = *pattern++) == EOS) {
                        c = '\\';
                        pattern--;
                    }
                }
                // FALLTHROUGH

            default:
                if (c != *string++) return FNM_NOMATCH;
                break;
        }
    }
}

static const char *rangematch(const char *pattern, int test, int flags) {
    char c, c2;
    int negate, ok;

    //
    // A bracket expression starting with an unquoted circumflex
    // character produces unspecified results (IEEE 1003.2-1992,
    // 3.13.2).  I have chosen to treat it like '!', for
    // consistancy with regular expression syntax.
    //

    if (negate = (*pattern == '!' || *pattern == '^')) pattern++;

    for (ok = 0; (c = *pattern++) != ']';) {
        if (c == '\\' && !(flags & FNM_NOESCAPE)) c = *pattern++;
        if (c == EOS) return NULL;

        if (*pattern == '-' && (c2 = *(pattern + 1)) != EOS && c2 != ']') {
            pattern += 2;
            if (c2 == '\\' && !(flags & FNM_NOESCAPE)) c2 = *pattern++;
            if (c2 == EOS) return NULL;
            if (c <= test && test <= c2) ok = 1;
        } else if (c == test) {
            ok = 1;
        }
    }

    return ok == negate ? NULL : pattern;
}
