//
// Assertion helper routine
//
#include <os.h>
#include <stdio.h>

void _assert(void *expr, void *filename, unsigned lineno) {
    printf("Assertion failed: %s, file %s, line %d\n", expr, filename, lineno);
    exit(3);
}
