/*
 * Internal definitions common to outelf32 and outelf64
 */
#ifndef OUTPUT_OUTELF_H
#define OUTPUT_OUTELF_H

#include "out/elf.h"

/* symbol binding */
#define SYM_GLOBAL      ELF32_ST_MKBIND(STB_GLOBAL)
#define SYM_LOCAL       ELF32_ST_MKBIND(STB_LOCAL)

#define GLOBAL_TEMP_BASE  0x40000000 /* bigger than any sane symbol index */

/* alignment of sections in file */
#define SEC_FILEALIGN 16

/* this stuff is needed for the stabs debugging format */
#define TY_STABSSYMLIN 0x40     /* ouch */

/* this stuff is needed for the dwarf debugging format */
#define TY_DEBUGSYMLIN 0x40     /* internal call to debug_out */

/* Known sections with nonstandard defaults */
struct elf_known_section {
    const char *name;   /* Name of section */
    int type;           /* Section type (SHT_) */
    uint32_t flags;     /* Section flags (SHF_) */
    uint32_t align;     /* Section alignment */
};
extern const struct elf_known_section elf_known_sections[];

/*
 * Special ELF sections (after the real sections but before debugging ones)
 */
#define sec_shstrtab            (nsects + 1)
#define sec_symtab              (nsects + 2)
#define sec_strtab              (nsects + 3)
#define sec_numspecial          3

/*
 * Debugging ELF sections (last in the file)
 */

/* stabs */
#define sec_stab                (nsections-3)
#define sec_stabstr             (nsections-2)
#define sec_rel_stab            (nsections-1)

/* stabs symbol table format */
struct stabentry {
    uint32_t n_strx;
    uint8_t n_type;
    uint8_t n_other;
    uint16_t n_desc;
    uint32_t n_value;
};

/* dwarf */
#define sec_debug_aranges       (nsections-10)
#define sec_rela_debug_aranges  (nsections-9)
#define sec_debug_pubnames      (nsections-8)
#define sec_debug_info          (nsections-7)
#define sec_rela_debug_info     (nsections-6)
#define sec_debug_abbrev        (nsections-5)
#define sec_debug_line          (nsections-4)
#define sec_rela_debug_line     (nsections-3)
#define sec_debug_frame         (nsections-2)
#define sec_debug_loc           (nsections-1)

void section_attrib(char *name, char *attr, int pass,
                    uint32_t *flags_and, uint32_t *flags_or,
                    uint64_t *align, int *type);

#define WRITE_STAB(p, n_strx, n_type, n_other, n_desc, n_value)  \
    do {                                                    \
        WRITELONG(p, n_strx);                               \
        WRITECHAR(p, n_type);                               \
        WRITECHAR(p, n_other);                              \
        WRITESHORT(p, n_desc);                              \
        WRITELONG(p, n_value);                              \
    } while (0)

#endif /* OUTPUT_OUTELF_H */
