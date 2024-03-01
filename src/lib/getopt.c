//
// Parse command options
//
#include <os.h>
#include <stdio.h>
#include <string.h>
#include <crtbase.h>
#include <getopt.h>

#define ERR(s, c) { if (opt->err) fprintf(stderr, "%s%s%c\n", argv[0], s, c); }

static struct opt *getoptvars() {
    struct process *proc = gettib()->proc;
    struct crtbase *crtbase = (struct crtbase *) proc->crtbase;

    return &crtbase->opt;
}

int *_opterr() {
    return &getoptvars()->err;
}

int *_optind() {
    return &getoptvars()->ind;
}

int *_optopt() {
    return &getoptvars()->opt;
}

char **_optarg() {
    return &getoptvars()->arg;
}

int getopt(int argc, char **argv, char *opts) {
    int c;
    char *cp;
    struct opt *opt = getoptvars();

    if (opt->sp == 1) {
        if (opt->ind >= argc || argv[opt->ind][0] != '-' || argv[opt->ind][1] == '\0') {
            return -1;
        } else if (strcmp(argv[opt->ind], "--") == 0) {
            opt->ind++;
            return -1;
        }
    }

    opt->opt = c = argv[opt->ind][opt->sp];
    if (c == ':' || (cp = strchr(opts, c)) == NULL) {
        ERR(": illegal option -- ", c)
        if (argv[opt->ind][++(opt->sp)] == '\0') {
            opt->ind++;
            opt->sp = 1;
        }

        return '?';
    }

    if (*++cp == ':') {
        if (argv[opt->ind][opt->sp + 1] != '\0') {
            opt->arg = &argv[opt->ind++][opt->sp + 1];
        } else if (++(opt->ind) >= argc) {
            ERR(": option requires an argument -- ", c)
            opt->sp = 1;
            return '?';
        } else {
            opt->arg = argv[opt->ind++];
        }

        opt->sp = 1;
    } else {
        if (argv[opt->ind][++(opt->sp)] == '\0') {
            opt->sp = 1;
            opt->ind++;
        }

        opt->arg = NULL;
    }

    return c;
}

static int getopt_internal(struct opt *opt, int argc, char **argv, const char *opts) {
    char *oli; // option letter list index

    if (opt->reset || !*opt->place) {
        // update scanning pointer
        opt->reset = 0;
        if (opt->ind >= argc || *(opt->place = argv[opt->ind]) != '-') {
            opt->place = "";
            return -1;
        }
        if (opt->place[1] && *++opt->place == '-') {
            // found "--"
            opt->place = "";
            return -2;
        }
    }

    // option letter okay?
    if ((opt->opt = *opt->place++) == ':' || !(oli = strchr(opts, opt->opt))) {
        // if the user didn't specify '-' as an option, assume it means -1
        if (opt->opt == '-') return -1;
        if (!*opt->place) ++opt->ind;
        if (opt->err && *opts != ':') {
            fprintf(stderr, "%s: illegal option -- %c\n", argv[0], opt->opt);
        }
        return '?';
    }

    if (*++oli != ':') {
        // don't need argument
        opt->arg = NULL;
        if (!*opt->place) ++opt->ind;
    } else {
        // need an argument
        if (*opt->place) {
            // no white space
            opt->arg = opt->place;
        } else if (argc <= ++opt->ind) {
            // no arg
            opt->place = "";
            if (opt->err && *opts != ':') {
                fprintf(stderr, "%s: option requires an argument -- %c\n", argv[0], opt->opt);
            }
            return ':';
        } else {
            // white space
            opt->arg = argv[opt->ind];
        }
        opt->place = "";
        ++opt->ind;
    }

    // dump back option letter
    return opt->opt;
}

int getopt_long(int argc, char **argv, const char *opts, const struct option *longopts, int *index) {
    int retval;
    struct opt *opt = getoptvars();

    if ((retval = getopt_internal(opt, argc, argv, opts)) == -2) {
        char *current_argv = argv[opt->ind++] + 2, *has_equal;
        int i, current_argv_len, match = -1;

        if (*current_argv == '\0') return -1;

        if ((has_equal = strchr(current_argv, '=')) != NULL) {
            current_argv_len = has_equal - current_argv;
            has_equal++;
        } else {
            current_argv_len = strlen(current_argv);
        }

        for (i = 0; longopts[i].name; i++) {
            if (strncmp(current_argv, longopts[i].name, current_argv_len)) continue;

            if (strlen(longopts[i].name) == current_argv_len) {
                match = i;
                break;
            }
            if (match == -1) match = i;
        }
        if (match != -1) {
            if (longopts[match].has_arg == required_argument ||
                longopts[match].has_arg == optional_argument) {
                if (has_equal) {
                    optarg = has_equal;
                } else {
                    optarg = argv[opt->ind++];
                }
            }
            if (longopts[match].has_arg == required_argument && optarg == NULL) {
                // Missing argument, leading: indicates no error should be generated
                if (opt->err && *opts != ':') {
                    fprintf(stderr, "%s: option requires an argument -- %s\n", argv[0], current_argv);
                }
                return ':';
            }
        } else {
            // No matching argument
            if (opt->err && *opts != ':') {
                fprintf(stderr, "%s: illegal option -- %s\n", argv[0], current_argv);
            }
            return '?';
        }
        if (longopts[match].flag) {
            *longopts[match].flag = longopts[match].val;
            retval = 0;
        } else {
            retval = longopts[match].val;
        }
        if (index) *index = match;
    }

    return retval;
}
