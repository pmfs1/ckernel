/*
 * Common code for outelf32 and outelf64
 */

#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include "as.h"
#include "out/outform.h"

#include "out/dwarf.h"
#include "out/elf.h"
#include "out/outelf.h"

#if defined(OF_ELF32) || defined(OF_ELF64)

const struct elf_known_section elf_known_sections[] = {
    { ".text",    SHT_PROGBITS, SHF_ALLOC|SHF_EXECINSTR,     16 },
    { ".rodata",  SHT_PROGBITS, SHF_ALLOC,                    4 },
    { ".lrodata", SHT_PROGBITS, SHF_ALLOC,                    4 },
    { ".data",    SHT_PROGBITS, SHF_ALLOC|SHF_WRITE,          4 },
    { ".ldata",   SHT_PROGBITS, SHF_ALLOC|SHF_WRITE,          4 },
    { ".bss",     SHT_NOBITS,   SHF_ALLOC|SHF_WRITE,          4 },
    { ".lbss",    SHT_NOBITS,   SHF_ALLOC|SHF_WRITE,          4 },
    { ".tdata",   SHT_PROGBITS, SHF_ALLOC|SHF_WRITE|SHF_TLS,  4 },
    { ".tbss",    SHT_NOBITS,   SHF_ALLOC|SHF_WRITE|SHF_TLS,  4 },
    { ".comment", SHT_PROGBITS, 0,                            1 },
    { NULL,       SHT_PROGBITS, SHF_ALLOC,                    1 } /* default */
};

/* parse section attributes */
void section_attrib(char *name, char *attr, int pass,
                    uint32_t *flags_and, uint32_t *flags_or,
                    uint64_t *align, int *type)
{
    char *opt, *val, *next;

    opt = as_skip_spaces(attr);
    if (!opt || !*opt)
        return;

    while ((opt = as_opt_val(opt, &val, &next))) {
        if (!as_stricmp(opt, "align")) {
            *align = atoi(val);
            if (*align == 0) {
                *align = SHA_ANY;
            } else if (!is_power2(*align)) {
                as_error(ERR_NONFATAL,
                           "section alignment %"PRId64" is not a power of two",
                           *align);
                *align = SHA_ANY;
            }
        } else if (!as_stricmp(opt, "alloc")) {
            *flags_and  |= SHF_ALLOC;
            *flags_or   |= SHF_ALLOC;
        } else if (!as_stricmp(opt, "noalloc")) {
            *flags_and  |= SHF_ALLOC;
            *flags_or   &= ~SHF_ALLOC;
        } else if (!as_stricmp(opt, "exec")) {
            *flags_and  |= SHF_EXECINSTR;
            *flags_or   |= SHF_EXECINSTR;
        } else if (!as_stricmp(opt, "noexec")) {
            *flags_and  |= SHF_EXECINSTR;
            *flags_or   &= ~SHF_EXECINSTR;
        } else if (!as_stricmp(opt, "write")) {
            *flags_and  |= SHF_WRITE;
            *flags_or   |= SHF_WRITE;
        } else if (!as_stricmp(opt, "tls")) {
            *flags_and  |= SHF_TLS;
            *flags_or   |= SHF_TLS;
        } else if (!as_stricmp(opt, "nowrite")) {
            *flags_and  |= SHF_WRITE;
            *flags_or   &= ~SHF_WRITE;
        } else if (!as_stricmp(opt, "progbits")) {
            *type = SHT_PROGBITS;
        } else if (!as_stricmp(opt, "nobits")) {
            *type = SHT_NOBITS;
        } else if (pass == 1) {
            as_error(ERR_WARNING,
                       "Unknown section attribute '%s' ignored on"
                       " declaration of section `%s'", opt, name);
        }
        opt = next;
    }
}

#endif /* defined(OF_ELF32) || defined(OF_ELF64) */
