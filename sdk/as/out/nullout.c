#include "as.h"
#include "aslib.h"
#include "out/outlib.h"

int null_setinfo(enum geninfo type, char **string) {
    (void) type;
    (void) string;
    return 0;
}

int null_directive(enum directives directive, char *value, int pass) {
    (void) directive;
    (void) value;
    (void) pass;
    return 0;
}

void null_sectalign(int32_t seg, unsigned int value) {
    (void) seg;
    (void) value;
}
