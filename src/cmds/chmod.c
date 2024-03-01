//
// Change file permissions
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <shlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

struct options {
    int recurse;
    int verbose;
    char *mode;
};

static int change_mode(char *path, struct options *opts) {
    struct stat st;
    int mode;

    // Get existing permissions for file
    if (stat(path, &st) < 0) {
        perror(path);
        return 1;
    }

    // Change permissions
    mode = parse_symbolic_mode(opts->mode, st.st_mode);
    if (mode == -1) {
        fprintf(stderr, "%s: invalid mode\n", opts->mode);
        return 1;
    }
    if (opts->verbose) {
        char perm[11];
        printf("mode of '%s' changed to %04o (%s)\n", path, mode & 07777, get_symbolic_mode(mode, perm));
    }
    if (chmod(path, mode) < 0) {
        perror(path);
        return 1;
    }

    // Recurse into sub-directories if requested
    if (opts->recurse && S_ISDIR(st.st_mode)) {
        struct dirent *dp;
        DIR *dirp;
        char *subdir;
        int rc;

        dirp = opendir(path);
        if (!dirp) {
            perror(path);
            return 1;
        }
        while ((dp = readdir(dirp))) {
            subdir = join_path(path, dp->d_name);
            if (!subdir) {
                fprintf(stderr, "error: out of memory\n");
                closedir(dirp);
                return 1;
            }
            rc = change_mode(subdir, opts);
            free(subdir);
            if (rc != 0) {
                closedir(dirp);
                return 1;
            }
        }
        closedir(dirp);
    }

    return 0;
}

static void usage() {
    fprintf(stderr, "usage: chmod [OPTIONS] MODE FILE...\n\n");
    fprintf(stderr, "  -R      Recursively change file permissions\n");
    fprintf(stderr, "  -v      Print out changed file permissions\n");
    exit(1);
}

shellcmd(chmod) {
        struct options opts;
        int c;
        int i;
        int rc;

        // Parse command line options
        memset(&opts, 0, sizeof(struct options));
        while ((c = getopt(argc, argv, "Rv?")) != EOF) {
            switch (c) {
                case 'R':
                    opts.recurse = 1;
                    break;

                case 'v':
                    opts.verbose = 1;
                    break;

                case '?':
                default:
                    usage();
            }
        }
        if (optind == argc) usage();
        opts.mode = argv[optind++];

        // Change file permissions
        for (i = optind; i < argc; i++) {
            rc = change_mode(argv[i], &opts);
            if (rc != 0) return 1;
        }

        return 0;
}
