/*
 * libout.c
 *
 * Common routines for the output backends.
 */

#include "compiler.h"
#include "as.h"
#include "out/outlib.h"

uint64_t realsize(enum out_type type, uint64_t size) {
    switch (type) {
        case OUT_REL1ADR:
            return 1;
        case OUT_REL2ADR:
            return 2;
        case OUT_REL4ADR:
            return 4;
        case OUT_REL8ADR:
            return 8;
        default:
            return size;
    }
}
