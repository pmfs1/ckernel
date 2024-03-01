#ifndef AS_QUOTE_H
#define AS_QUOTE_H

#include "compiler.h"

char *as_quote(char *str, size_t len);

size_t as_unquote(char *str, char **endptr);

char *as_skip_string(char *str);

#endif /* AS_QUOTE_H */
