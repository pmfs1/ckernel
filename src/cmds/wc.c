//
// Word, line, and character count
//
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <shlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/stat.h>

struct options {
    int charcount;
    int wordcount;
    int linecount;
};

struct stats {
    int chars;
    int words;
    int lines;
};

static void process_file(int fd, struct stats *s) {
    char buffer[4096];
    int n;
    int inword = 0;
    char *p, *end;

    memset(s, 0, sizeof(struct stats));
    while ((n = read(fd, buffer, sizeof(buffer))) > 0) {
        p = buffer;
        end = p + n;
        while (p < end) {
            int c = *p++;
            if (isspace(c)) {
                inword = 0;
            } else {
                if (!inword) s->words++;
                inword = 1;
            }
            if (c == '\n') s->lines++;
        }
        s->chars += n;
    }
}

static void print_stats(struct stats *s, char *name, struct options *opts) {
    if (opts->linecount) printf(" %6d", s->lines);
    if (opts->wordcount) printf(" %6d", s->words);
    if (opts->charcount) printf(" %6d", s->chars);
    printf(" %s\n", name);
}

static void usage() {
    fprintf(stderr, "usage: wc [OPTIONS] FILE...\n\n");
    fprintf(stderr, "  -c      Output character count\n");
    fprintf(stderr, "  -l      Output line count\n");
    fprintf(stderr, "  -w      Output word count\n");
    exit(1);
}

shellcmd(wc) {
        struct options opts;
        int c;
        int fd;
        int i;
        struct stats s;
        struct stats total;

        // Parse command line options
        memset(&opts, 0, sizeof(struct options));
        while ((c = getopt(argc, argv, "clw?")) != EOF) {
            switch (c) {
                case 'c':
                    opts.charcount = 1;
                    break;

                case 'l':
                    opts.linecount = 1;
                    break;

                case 'w':
                    opts.wordcount = 1;
                    break;

                case '?':
                default:
                    usage();
            }
        }
        if (!opts.charcount && !opts.linecount && !opts.wordcount) {
            opts.charcount = opts.linecount = opts.wordcount = 1;
        }

        memset(&total, 0, sizeof(struct stats));
        if (optind == argc) {
            // Read from stdin
            process_file(fileno(stdin), &s);
            print_stats(&s, "", &opts);
        } else {
            // Process all files
            for (i = optind; i < argc; i++) {
                fd = open(argv[i], O_RDONLY);
                if (fd < 0) {
                    perror(argv[i]);
                    continue;
                }
                process_file(fd, &s);
                close(fd);
                print_stats(&s, argv[i], &opts);
                total.chars += s.chars;
                total.words += s.words;
                total.lines += s.lines;
            }

            // Output totals
            if (argc - optind > 1) {
                print_stats(&total, "total", &opts);
            }
        }

        return 0;
}
