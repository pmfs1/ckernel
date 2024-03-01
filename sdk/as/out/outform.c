/*
 * outform.c	manages a list of output formats, and associates
 *		them with their relevant drivers. Also has a
 *		routine to find the correct driver given a name
 *		for it
 */

#include "compiler.h"

#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define BUILD_DRIVERS_ARRAY

#include "out/outform.h"

struct ofmt *ofmt_find(char *name, struct ofmt_alias **ofmt_alias) {
    struct ofmt **ofp, *of;
    unsigned int i;

    *ofmt_alias = NULL;

    /* primary targets first */
    for (ofp = drivers; (of = *ofp); ofp++) {
        if (!as_stricmp(name, of->shortname))
            return of;
    }

    /* lets walk thru aliases then */
    for (i = 0; i < ARRAY_SIZE(ofmt_aliases); i++) {
        if (ofmt_aliases[i].shortname &&
            !as_stricmp(name, ofmt_aliases[i].shortname)) {
            *ofmt_alias = &ofmt_aliases[i];
            return ofmt_aliases[i].ofmt;
        }
    }

    return NULL;
}

struct dfmt *dfmt_find(struct ofmt *ofmt, char *name) {
    struct dfmt **dfp, *df;

    for (dfp = ofmt->debug_formats; (df = *dfp); dfp++) {
        if (!as_stricmp(name, df->shortname))
            return df;
    }
    return NULL;
}

void ofmt_list(struct ofmt *deffmt, FILE *fp) {
    struct ofmt **ofp, *of;
    unsigned int i;

    /* primary targets first */
    for (ofp = drivers; (of = *ofp); ofp++) {
        fprintf(fp, "  %c %-10s%s\n",
                of == deffmt ? '*' : ' ',
                of->shortname, of->fullname);
    }

    /* lets walk through aliases then */
    for (i = 0; i < ARRAY_SIZE(ofmt_aliases); i++) {
        if (!ofmt_aliases[i].shortname)
            continue;
        fprintf(fp, "    %-10s%s\n",
                ofmt_aliases[i].shortname,
                ofmt_aliases[i].fullname);
    }
}

void dfmt_list(struct ofmt *ofmt, FILE *fp) {
    struct dfmt **dfp, *df;

    for (dfp = ofmt->debug_formats; (df = *dfp); dfp++) {
        fprintf(fp, "  %c %-10s%s\n",
                df == ofmt->current_dfmt ? '*' : ' ',
                df->shortname, df->fullname);
    }
}
