/* 
 * parser.h   header file for the parser module of the Netwide
 *            Assembler
 */

#ifndef AS_PARSER_H
#define AS_PARSER_H

void parser_global_info(struct location *locp);

insn *parse_line(int pass, char *buffer, insn *result, ldfunc ldef);

void cleanup_insn(insn *instruction);

#endif
