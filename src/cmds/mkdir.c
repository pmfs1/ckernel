//
// Make directories
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <shlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

struct options {
    int makepath;
    char *mode;
};

static int make_directory(char *path, struct options *opts) {
    char *p;
    int mode;

    if (opts->mode) {
        mode = parse_symbolic_mode(opts->mode, 0);
        if (mode == -1) {
            fprintf(stderr, "%s: invalid mode\n", opts->mode);
            return 1;
        }
    } else {
        mode = 0777;
    }

    if (mkdir(path, mode) < 0) {
        if (!opts->makepath) {
            perror(path);
            return 1;
        }

        p = path;
        while (p) {
            p = strchr(p, '/');
            if (p) *p = 0;
            if (mkdir(path, mode) < 0 && errno != EEXIST) {
                perror(path);
                return 1;
            }
            if (p) *p++ = '/';
        }
    }

    return 0;
}

static void usage() {
    fprintf(stderr, "usage: mkdir [OPTIONS] DIR...\n\n");
    fprintf(stderr, "  -p      Create any missing intermediate pathname components\n");
    fprintf(stderr, "  -m mode Set file permission mode\n");
    exit(1);
}

shellcmd(mkdir) {
        struct options opts;
        int c;
        int i;
        int rc;

        // Parse command line options
        memset(&opts, 0, sizeof(struct options));
        while ((c = getopt(argc, argv, "pm:?")) != EOF) {
            switch (c) {
                case 'p':
                    opts.makepath = 1;
                    break;

                case 'm':
                    opts.mode = optarg;
                    break;

                case '?':
                default:
                    usage();
            }
        }
        if (optind == argc) usage();

        for (i = optind; i < argc; i++) {
            rc = make_directory(argv[i], &opts);
            if (rc != 0) return 1;
        }

        return 0;
}
