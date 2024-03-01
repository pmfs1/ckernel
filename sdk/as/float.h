/* 
 * float.h   header file for the floating-point constant module of
 *	     the assembler
 */

#ifndef AS_FLOAT_H
#define AS_FLOAT_H

#include "as.h"

enum float_round {
    FLOAT_RC_NEAR,
    FLOAT_RC_ZERO,
    FLOAT_RC_DOWN,
    FLOAT_RC_UP,
};

int float_const(const char *string, int sign, uint8_t *result, int bytes,
                efunc error);

int float_option(const char *option);

#endif
