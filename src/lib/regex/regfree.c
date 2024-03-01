//
// Free regular expression
//
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "regex2.h"

//
// regfree - free everything
//
void regfree(regex_t *preg) {
    struct re_guts *g;

    if (preg->re_magic != MAGIC1) return; // oops
    g = preg->re_g;
    if (g == NULL || g->magic != MAGIC2) return; // oops again

    preg->re_magic = 0;   // mark it invalid
    g->magic = 0;     // mark it invalid

    if (g->strip != NULL) free(g->strip);
    if (g->sets != NULL) free(g->sets);
    if (g->setbits != NULL) free(g->setbits);
    if (g->must != NULL) free(g->must);
    free(g);
}
