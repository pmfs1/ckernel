#include "cc.h"

// State for current compilation.
CCState *cc_state;

// display some information during compilation
int verbose = 0;

// Compile with debug symbol (and use them if error during execution)
int do_debug = 0;

// Number of token types, including identifiers and strings
int tok_ident;

// Path to the C runtime libraries
const char *cc_lib_path = CONFIG_CCDIR;

// Display benchmark infos
static int do_bench = 0;

#define WD_ALL            0x0001  // Warning is activated when using -Wall
#define FD_INVERT         0x0002  // Invert value before storing

typedef struct FlagDef {
    uint16_t offset;
    uint16_t flags;
    const char *name;
} FlagDef;

static const FlagDef warning_defs[] = {
        {offsetof(CCState, warn_unsupported),   0,                      "unsupported"},
        {offsetof(CCState, warn_write_strings), 0,                      "write-strings"},
        {offsetof(CCState, warn_error),         0,                      "error"},
        {offsetof(CCState, warn_implicit_function_declaration), WD_ALL, "implicit-function-declaration"},
};

static const FlagDef flag_defs[] = {
        {offsetof(CCState, char_is_unsigned),   0,       "unsigned-char"},
        {offsetof(CCState, char_is_unsigned), FD_INVERT, "signed-char"},
        {offsetof(CCState, nocommon),         FD_INVERT, "common"},
        {offsetof(CCState, leading_underscore), 0,       "leading-underscore"},
};

#define CC_OPTION_HAS_ARG 0x0001
#define CC_OPTION_NOSEP   0x0002 // Cannot have space before option and arg

typedef struct CCOption {
    const char *name;
    uint16_t index;
    uint16_t flags;
} CCOption;

enum {
    CC_OPTION_HELP,
    CC_OPTION_I,
    CC_OPTION_D,
    CC_OPTION_U,
    CC_OPTION_L,
    CC_OPTION_B,
    CC_OPTION_l,
    CC_OPTION_bench,
    CC_OPTION_g,
    CC_OPTION_c,
    CC_OPTION_static,
    CC_OPTION_shared,
    CC_OPTION_soname,
    CC_OPTION_entry,
    CC_OPTION_fixed,
    CC_OPTION_filealign,
    CC_OPTION_stub,
    CC_OPTION_def,
    CC_OPTION_o,
    CC_OPTION_r,
    CC_OPTION_Wl,
    CC_OPTION_W,
    CC_OPTION_m,
    CC_OPTION_f,
    CC_OPTION_nofll,
    CC_OPTION_noshare,
    CC_OPTION_nostdinc,
    CC_OPTION_nostdlib,
    CC_OPTION_print_search_dirs,
    CC_OPTION_rdynamic,
    CC_OPTION_v,
    CC_OPTION_w,
    CC_OPTION_E,
};

static const CCOption cc_options[] = {
        {"h",                 CC_OPTION_HELP,              0},
        {"?",                 CC_OPTION_HELP,              0},
        {"I",                 CC_OPTION_I,         CC_OPTION_HAS_ARG},
        {"D",                 CC_OPTION_D,         CC_OPTION_HAS_ARG},
        {"U",                 CC_OPTION_U,         CC_OPTION_HAS_ARG},
        {"L",                 CC_OPTION_L,         CC_OPTION_HAS_ARG},
        {"B",                 CC_OPTION_B,         CC_OPTION_HAS_ARG},
        {"l",                 CC_OPTION_l,         CC_OPTION_HAS_ARG | CC_OPTION_NOSEP},
        {"bench",             CC_OPTION_bench,             0},
        {"g",                 CC_OPTION_g,         CC_OPTION_HAS_ARG | CC_OPTION_NOSEP},
        {"c",                 CC_OPTION_c,                 0},
        {"static",            CC_OPTION_static,            0},
        {"shared",            CC_OPTION_shared,            0},
        {"soname",            CC_OPTION_soname,    CC_OPTION_HAS_ARG},
        {"entry",             CC_OPTION_entry,     CC_OPTION_HAS_ARG},
        {"fixed",             CC_OPTION_fixed,     CC_OPTION_HAS_ARG},
        {"filealign",         CC_OPTION_filealign, CC_OPTION_HAS_ARG},
        {"stub",              CC_OPTION_stub,      CC_OPTION_HAS_ARG},
        {"def",               CC_OPTION_def,       CC_OPTION_HAS_ARG},
        {"o",                 CC_OPTION_o,         CC_OPTION_HAS_ARG},
        {"rdynamic",          CC_OPTION_rdynamic,          0},
        {"r",                 CC_OPTION_r,                 0},
        {"Wl,",               CC_OPTION_Wl,        CC_OPTION_HAS_ARG | CC_OPTION_NOSEP},
        {"W",                 CC_OPTION_W,         CC_OPTION_HAS_ARG | CC_OPTION_NOSEP},
        {"m",                 CC_OPTION_m,         CC_OPTION_HAS_ARG},
        {"f",                 CC_OPTION_f,         CC_OPTION_HAS_ARG | CC_OPTION_NOSEP},
        {"nofll",             CC_OPTION_nofll,             0},
        {"noshare",           CC_OPTION_noshare,           0},
        {"nostdinc",          CC_OPTION_nostdinc,          0},
        {"nostdlib",          CC_OPTION_nostdlib,          0},
        {"print-search-dirs", CC_OPTION_print_search_dirs, 0},
        {"v",                 CC_OPTION_v,         CC_OPTION_HAS_ARG | CC_OPTION_NOSEP},
        {"w",                 CC_OPTION_w,                 0},
        {"E",                 CC_OPTION_E,                 0},
        {NULL},
};

static char **files;
static int nb_files, nb_libraries;
static int multiple_files;
static int print_search_dirs;
static int output_type;
static int reloc_output;
static const char *outfile;

static int set_flag(CCState *s, const FlagDef *flags, int nb_flags, const char *name, int value) {
    int i;
    const FlagDef *p;
    const char *r;

    r = name;
    if (r[0] == 'n' && r[1] == 'o' && r[2] == '-') {
        r += 3;
        value = !value;
    }
    for (i = 0, p = flags; i < nb_flags; i++, p++) {
        if (!strcmp(r, p->name)) goto found;
    }
    return -1;
    found:
    if (p->flags & FD_INVERT) value = !value;
    *(int *) ((uint8_t *) s + p->offset) = value;
    return 0;
}

// Set/reset a warning
int cc_set_warning(CCState *s, const char *warning_name, int value) {
    int i;
    const FlagDef *p;

    if (!strcmp(warning_name, "all")) {
        for (i = 0, p = warning_defs; i < countof(warning_defs); i++, p++) {
            if (p->flags & WD_ALL) *(int *) ((uint8_t *) s + p->offset) = 1;
        }
        return 0;
    } else {
        return set_flag(s, warning_defs, countof(warning_defs), warning_name, value);
    }
}

// Set/reset a flag
int cc_set_flag(CCState *s, const char *flag_name, int value) {
    return set_flag(s, flag_defs, countof(flag_defs), flag_name, value);
}

static int64_t getclock_us(void) {
#ifdef _WIN32
    struct _timeb tb;
    _ftime(&tb);
    return (tb.time * 1000LL + tb.millitm) * 1000LL;
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * INT64_C(1000000) + tv.tv_usec;
#endif
}

void help(void) {
    printf("cc version " CC_VERSION "\n"
           "usage: cc [-v] [-c] [-o outfile] [-Bdir] [-bench] [-Idir] [-Dsym[=val]] [-Usym]\n"
           "          [-Wwarn] [-g] [-Ldir] [-llib] [-shared] [-soname name]\n"
           "          [-static] [infile1 infile2...]\n"
           "\n"
           "General options:\n"
           "  -v           display current version, increase verbosity\n"
           "  -c           compile only - generate an object file\n"
           "  -o outfile   set output filename\n"
           "  -B dir       set cc internal library path\n"
           "  -bench       output compilation statistics\n"
           "  -fflag       set or reset (with 'no-' prefix) 'flag' (see man page)\n"
           "  -Wwarning    set or reset (with 'no-' prefix) 'warning' (see man page)\n"
           "  -w           disable all warnings\n"
           "  -g           generate runtime debug info\n"
           "Preprocessor options:\n"
           "  -E           preprocess only\n"
           "  -Idir        add include path 'dir'\n"
           "  -Dsym[=val]  define 'sym' with value 'val'\n"
           "  -Usym        undefine 'sym'\n"
           "Linker options:\n"
           "  -Ldir        add library path 'dir'\n"
           "  -llib        link with dynamic or static library 'lib'\n"
           "  -shared      generate a shared library\n"
           "  -soname      set name for shared library to be used at runtime\n"
           "  -entry sym   set start symbol name\n"
           "  -fixed addr  set base address (and do not generate relocation info)\n"
           "  -filealign n alignment for sections in PE file\n"
           "  -stub file   set DOS stub for PE file\n"
           "  -def file    generate import definition file for shared library\n"
           "  -static      static linking\n"
           "  -rdynamic    export all global symbols to dynamic linker\n"
           "  -r           generate (relocatable) object file\n"
           "  -m mapfile   generate linker map file\n"
    );
}

int parse_arguments(CCState *s, int argc, char **argv) {
    int oind;
    const CCOption *popt;
    const char *oarg, *p1, *r1;
    char *r;

    oind = 0;
    while (1) {
        if (oind >= argc) {
            if (nb_files == 0 && !print_search_dirs) {
                if (verbose) exit(0);
                goto show_help;
            }
            break;
        }
        r = argv[oind++];
        if (r[0] != '-' || r[1] == '\0') {
            // Add a new file
            dynarray_add((void ***) &files, &nb_files, r);
            if (!multiple_files) {
                oind--;
                // argv[0] will be this file
                break;
            }
        } else {
            // Find option in table (match only the first chars)
            popt = cc_options;
            for (;;) {
                p1 = popt->name;
                if (p1 == NULL) error("invalid option -- '%s'", r);
                r1 = r + 1;
                for (;;) {
                    if (*p1 == '\0') goto option_found;
                    if (*r1 != *p1) break;
                    p1++;
                    r1++;
                }
                popt++;
            }
            option_found:
            if (popt->flags & CC_OPTION_HAS_ARG) {
                if (*r1 != '\0' || (popt->flags & CC_OPTION_NOSEP)) {
                    oarg = r1;
                } else {
                    if (oind >= argc) error("argument to '%s' is missing", r);
                    oarg = argv[oind++];
                }
            } else {
                if (*r1 != '\0') goto show_help;
                oarg = NULL;
            }

            switch (popt->index) {
                case CC_OPTION_HELP:
                show_help:
                    help();
                    exit(1);
                case CC_OPTION_I:
                    if (cc_add_include_path(s, oarg) < 0) error("too many include paths");
                    break;
                case CC_OPTION_D: {
                    char *sym, *value;
                    sym = (char *) oarg;
                    value = strchr(sym, '=');
                    if (value) {
                        *value = '\0';
                        value++;
                    }
                    cc_define_symbol(s, sym, value);
                    break;
                }
                case CC_OPTION_U:
                    cc_undefine_symbol(s, oarg);
                    break;
                case CC_OPTION_L:
                    cc_add_library_path(s, oarg);
                    break;
                case CC_OPTION_B:
                    // Set cc utilities path (mainly for cc development)
                    cc_lib_path = oarg;
                    break;
                case CC_OPTION_l:
                    dynarray_add((void ***) &files, &nb_files, r);
                    nb_libraries++;
                    break;
                case CC_OPTION_bench:
                    do_bench = 1;
                    break;
                case CC_OPTION_g:
                    do_debug = 1;
                    break;
                case CC_OPTION_c:
                    multiple_files = 1;
                    output_type = CC_OUTPUT_OBJ;
                    break;
                case CC_OPTION_nofll:
                    s->nofll = 1;
                    break;
                case CC_OPTION_static:
                    s->static_link = 1;
                    break;
                case CC_OPTION_shared:
                    output_type = CC_OUTPUT_DLL;
                    break;
                case CC_OPTION_soname:
                    s->soname = oarg;
                    break;
                case CC_OPTION_entry:
                    s->start_symbol = oarg;
                    break;
                case CC_OPTION_fixed:
                    s->imagebase = strtoul(oarg, NULL, 0);
                    break;
                case CC_OPTION_filealign:
                    s->filealign = strtoul(oarg, NULL, 0);
                    break;
                case CC_OPTION_stub:
                    s->stub = oarg;
                    break;
                case CC_OPTION_def:
                    s->def_file = oarg;
                    break;
                case CC_OPTION_o:
                    multiple_files = 1;
                    outfile = oarg;
                    break;
                case CC_OPTION_m:
                    s->mapfile = oarg;
                    break;
                case CC_OPTION_r:
                    // Generate a .o merging several output files
                    reloc_output = 1;
                    output_type = CC_OUTPUT_OBJ;
                    break;
                case CC_OPTION_noshare:
                    s->noshare = 1;
                    break;
                case CC_OPTION_nostdinc:
                    s->nostdinc = 1;
                    break;
                case CC_OPTION_nostdlib:
                    s->nostdlib = 1;
                    break;
                case CC_OPTION_print_search_dirs:
                    print_search_dirs = 1;
                    break;
                case CC_OPTION_v:
                    do {
                        if (verbose++ == 0) printf("cc version %s\n", CC_VERSION);
                    } while (*oarg++ == 'v');
                    break;
                case CC_OPTION_f:
                    if (cc_set_flag(s, oarg, 1) < 0 && s->warn_unsupported) goto unsupported_option;
                    break;
                case CC_OPTION_W:
                    if (cc_set_warning(s, oarg, 1) < 0 && s->warn_unsupported) goto unsupported_option;
                    break;
                case CC_OPTION_w:
                    s->warn_none = 1;
                    break;
                case CC_OPTION_rdynamic:
                    s->rdynamic = 1;
                    break;
                case CC_OPTION_Wl: {
                    const char *p;
                    if (strstart(oarg, "-Ttext,", &p)) {
                        s->text_addr = strtoul(p, NULL, 16);
                        s->has_text_addr = 1;
                    } else if (strstart(oarg, "--oformat,", &p)) {
                        if (strstart(p, "elf32-", NULL)) {
                            s->output_format = CC_OUTPUT_FORMAT_ELF;
                        } else if (!strcmp(p, "binary")) {
                            s->output_format = CC_OUTPUT_FORMAT_BINARY;
                        } else {
                            error("target %s not found", p);
                        }
                    } else {
                        error("unsupported linker option '%s'", oarg);
                    }
                    break;
                }
                case CC_OPTION_E:
                    output_type = CC_OUTPUT_PREPROCESS;
                    break;
                default:
                    if (s->warn_unsupported) {
                        unsupported_option:
                        warning("unsupported option '%s'", r);
                    }
            }
        }
    }
    return oind;
}

int main(int argc, char **argv) {
    int i;
    CCState *s;
    int nb_objfiles, ret, oind;
    char objfilename[1024];
    int64_t start_time = 0;
    char *alt_lib_path;

    init_util();
    s = cc_new();
    output_type = CC_OUTPUT_EXE;
    outfile = NULL;
    multiple_files = 1;
    files = NULL;
    nb_files = 0;
    nb_libraries = 0;
    reloc_output = 0;
    print_search_dirs = 0;
    ret = 0;

    oind = parse_arguments(s, argc - 1, argv + 1) + 1;

    alt_lib_path = getenv("CCPREFIX");
    if (alt_lib_path) cc_lib_path = alt_lib_path;
    if (print_search_dirs) {
        printf("install: %s/\n", cc_lib_path);
        return 0;
    }

    nb_objfiles = nb_files - nb_libraries;

    // Check -c consistency: only single file handled.
    // TODO: check file type
    if (output_type == CC_OUTPUT_OBJ && !reloc_output) {
        // Accepts only a single input file
        if (nb_objfiles != 1) error("cannot specify multiple files with -c");
        if (nb_libraries != 0) error("cannot specify libraries with -c");
    }

    if (output_type == CC_OUTPUT_PREPROCESS) {
        if (!outfile) {
            s->outfile = stdout;
        } else {
            s->outfile = fopen(outfile, "w");
            if (!s->outfile) error("could not open '%s", outfile);
        }
    } else {
        if (!outfile) {
            // Compute default outfile name
            char *ext;
            const char *name = strcmp(files[0], "-") == 0 ? "a" : cc_basename(files[0]);
            pstrcpy(objfilename, sizeof(objfilename), name);
            ext = cc_fileextension(objfilename);

            if (output_type == CC_OUTPUT_DLL) {
                strcpy(ext, ".dll");
            } else if (output_type == CC_OUTPUT_EXE) {
                strcpy(ext, ".exe");
            } else if (output_type == CC_OUTPUT_OBJ && !reloc_output && *ext) {
                strcpy(ext, ".o");
            } else {
                pstrcpy(objfilename, sizeof(objfilename), "a.out");
            }
            outfile = objfilename;
        }
    }

    if (do_bench) {
        start_time = getclock_us();
    }

    cc_set_output_type(s, output_type);

    // Compile or add each files or library
    for (i = 0; i < nb_files && ret == 0; i++) {
        const char *filename;

        filename = files[i];
        if (output_type == CC_OUTPUT_PREPROCESS) {
            if (cc_add_file_ex(s, filename, AFF_PRINT_ERROR | AFF_PREPROCESS) < 0) ret = 1;
        } else if (filename[0] == '-' && filename[1]) {
            if (cc_add_library(s, filename + 2) < 0) error("cannot find %s", filename);
        } else {
            if (verbose == 1) printf("-> %s\n", filename);
            if (cc_add_file(s, filename) < 0) ret = 1;
        }
    }

    // Free all files
    cc_free(files);
    if (ret) goto cleanup;

    if (do_bench) {
        double total_time;
        total_time = (double) (getclock_us() - start_time) / 1000000.0;
        if (total_time < 0.001) total_time = 0.001;
        if (total_bytes < 1) total_bytes = 1;
        printf("%d idents, %d lines, %d bytes, %0.3f s, %d lines/s, %0.1f MB/s\n",
               tok_ident - TOK_IDENT, total_lines, total_bytes,
               total_time, (int) (total_lines / total_time),
               total_bytes / total_time / 1000000.0);
    }

    if (s->output_type == CC_OUTPUT_PREPROCESS) {
        if (outfile) fclose(s->outfile);
    } else if (s->output_type != CC_OUTPUT_OBJ) {
        ret = pe_output_file(s, outfile);
    } else {
        ret = cc_output_file(s, outfile) ? 1 : 0;
    }

    cleanup:
    cc_delete(s);

#ifdef MEM_DEBUG
    if (do_bench) {
      printf("memory: %d bytes, max = %d bytes\n", mem_cur_size, mem_max_size);
    }
#endif
    return ret;
}
