/*
 * assemble.h  header file for assemble.c
 */

#ifndef AS_ASSEMBLE_H
#define AS_ASSEMBLE_H

int64_t insn_size(int32_t segment, int64_t offset, int bits, uint32_t cp,
                  insn *instruction, efunc error);

int64_t assemble(int32_t segment, int64_t offset, int bits, uint32_t cp,
                 insn *instruction, struct ofmt *output, efunc error,
                 ListGen *listgen);

#endif
