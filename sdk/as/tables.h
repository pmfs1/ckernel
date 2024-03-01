/*
 * tables.h
 *
 * Declarations for auto-generated tables
 */

#ifndef AS_TABLES_H
#define AS_TABLES_H

#include "compiler.h"
#include <inttypes.h>
#include "insnsi.h"        /* For enum opcode */
#include "opflags.h"        /* For opflags_t */

/* --- From standard.mac via macros.pl: --- */

/* macros.c */
extern const unsigned char as_stdmac[];
extern const unsigned char *const as_stdmac_after_tasm;

const unsigned char *as_stdmac_find_package(const char *);

/* --- From insns.dat via insns.pl: --- */

/* insnsn.c */
extern const char *const as_insn_names[];

/* --- From regs.dat via regs.pl: --- */

/* regs.c */
extern const char *const as_reg_names[];
/* regflags.c */
extern const opflags_t as_reg_flags[];
/* regvals.c */
extern const int as_regvals[];

#endif /* AS_TABLES_H */
