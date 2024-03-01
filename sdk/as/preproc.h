/* 
 * preproc.h  header file for preproc.c
 */

#ifndef AS_PREPROC_H
#define AS_PREPROC_H

#include "pptok.h"

extern const char *const pp_directives[];
extern const uint8_t pp_directives_len[];

/* Pointer to a macro chain */
typedef const unsigned char macros_t;

enum preproc_token pp_token_hash(const char *token);

void pp_include_path(char *);

void pp_pre_include(char *);

void pp_pre_define(char *);

void pp_pre_undefine(char *);

void pp_runtime(char *);

void pp_extra_stdmac(macros_t *);

#endif
