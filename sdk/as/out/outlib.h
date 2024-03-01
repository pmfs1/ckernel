#ifndef AS_OUTLIB_H
#define AS_OUTLIB_H

#include "as.h"

uint64_t realsize(enum out_type type, uint64_t size);

/* Do-nothing versions of some output routines */
int null_setinfo(enum geninfo type, char **string);

int null_directive(enum directives directive, char *value, int pass);

void null_sectalign(int32_t seg, unsigned int value);

/* Do-nothing versions of all the debug routines */
struct ofmt;

void null_debug_init(void);

void null_debug_linenum(const char *filename, int32_t linenumber,
                        int32_t segto);

void null_debug_deflabel(char *name, int32_t segment, int64_t offset,
                         int is_global, char *special);

void null_debug_directive(const char *directive, const char *params);

void null_debug_typevalue(int32_t type);

void null_debug_output(int type, void *param);

void null_debug_cleanup(void);

extern struct dfmt *null_debug_arr[2];

#endif /* AS_OUTLIB_H */
