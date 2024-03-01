#include "as.h"
#include "version.h"

/* This is printed when entering as -v */
const char as_version[] = AS_VER;
const char as_date[] = __DATE__;
const char as_compile_options[] = ""
#ifdef DEBUG
" with -DDEBUG"
#endif
;

/* These are used by some backends. */
const char as_comment[] =
        "The assembler " AS_VER;

const char as_signature[] =
        "AS " AS_VER;
