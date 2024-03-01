//
// Estimate file space usage
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <shlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>

struct options {
    int blksize;
    int all;
};

static int display_file_size(char *path, struct options *opts) {
    struct stat st;
    int total = 0;

    // Stat file
    if (stat(path ? path : ".", &st) < 0) {
        perror(path);
        return -1;
    }

    if (S_ISDIR(st.st_mode)) {
        struct dirent *dp;
        DIR *dirp;
        char *fn;
        int size;

        dirp = opendir(path);
        if (!dirp) {
            perror(path);
            return -1;
        }
        while ((dp = readdir(dirp))) {
            fn = join_path(path, dp->d_name);
            if (!fn) {
                fprintf(stderr, "error: out of memory\n");
                closedir(dirp);
                return -1;
            }
            size = display_file_size(fn, opts);
            free(fn);
            if (size == -1) {
                closedir(dirp);
                return -1;
            }
            total += size;
        }
        closedir(dirp);
    } else {
        total = st.st_size;
    }

    if (opts->all || S_ISDIR(st.st_mode)) {
        printf("%d\t%s\n", (total + opts->blksize - 1) / opts->blksize, path);
    }

    return total;
}

static void usage() {
    fprintf(stderr, "usage: du [OPTIONS] FILE...\n\n");
    fprintf(stderr, "  -a      Output both file and directories\n");
    fprintf(stderr, "  -b      Write files size in units of 512-byte blocks\n");
    fprintf(stderr, "  -k      Write files size in units of 1024-byte blocks\n");
    exit(1);
}

shellcmd(du) {
        struct options opts;
        int c;
        int rc;
        int i;

        // Parse command line options
        memset(&opts, 0, sizeof(struct options));
        opts.blksize = 1024;
        while ((c = getopt(argc, argv, "abk?")) != EOF) {
            switch (c) {
                case 'a':
                    opts.all = 1;
                    break;

                case 'b':
                    opts.blksize = 512;
                    break;

                case 'k':
                    opts.blksize = 1024;
                    break;

                case '?':
                default:
                    usage();
            }
        }

        if (optind == argc) {
            rc = display_file_size(".", &opts);
        } else {
            for (i = optind; i < argc; i++) {
                rc = display_file_size(argv[i], &opts);
                if (rc < 0) break;
            }
        }

        return rc < 0;
}
