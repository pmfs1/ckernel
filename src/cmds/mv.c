//
// Move files
//
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <shlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

struct options {
    int force;
    int verbose;
};

static void usage() {
    fprintf(stderr, "usage: mv [OPTIONS] SRC DEST\n");
    fprintf(stderr, "       mv [OPTIONS] SRC... DIR\n\n");
    fprintf(stderr, "  -f      Force overwriting of destination files\n");
    fprintf(stderr, "  -v      Print files being moved\n");
    exit(1);
}

shellcmd(mv) {
        struct options opts;
        int c;
        char *dest;
        int dirdest;
        struct stat st;
        int rc;
        int i;

        // Parse command line options
        memset(&opts, 0, sizeof(struct options));
        while ((c = getopt(argc, argv, "fv?")) != EOF) {
            switch (c) {
                case 'f':
                    opts.force = 1;
                    break;

                case 'v':
                    opts.verbose = 1;
                    break;

                case '?':
                default:
                    usage();
            }
        }
        if (argc - optind < 2) usage();

        // Check if destination is a directory
        dest = argv[argc - 1];
        dirdest = stat(dest, &st) >= 0 && S_ISDIR(st.st_mode);

        if (argc - optind > 2) {
            // Target must be a directory
            if (!dirdest) {
                fprintf(stderr, "%s: is not a directory\n", dest);
                return 1;
            }

            // Move source files to destination directory
            for (i = optind; i < argc - 1; i++) {
                char *destfn = join_path(dest, argv[i]);
                if (opts.force) unlink(destfn);
                if (opts.verbose) printf("move '%s' to '%s'\n", argv[i], destfn);
                rc = rename(argv[i], destfn);
                if (rc < 0) {
                    perror(destfn);
                    free(destfn);
                    return 1;
                }
                free(destfn);
            }
        } else {
            // Rename source file to destination
            if (opts.force) unlink(argv[optind + 1]);
            if (opts.verbose) printf("move '%s' to '%s'\n", argv[optind], argv[optind + 1]);
            rc = rename(argv[optind], argv[optind + 1]);
            if (rc < 0) {
                perror(argv[optind]);
                return 1;
            }
        }

        return 0;
}
