/*
 * listing.c    listing file generator for the assembler
 */

#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <inttypes.h>

#include "as.h"
#include "aslib.h"
#include "listing.h"

#define LIST_MAX_LEN 216        /* something sensible */
#define LIST_INDENT  40
#define LIST_HEXBIT  18

typedef struct MacroInhibit MacroInhibit;

static struct MacroInhibit {
    MacroInhibit *next;
    int level;
    int inhibiting;
} *mistack;

static char xdigit[] = "0123456789ABCDEF";

#define HEX(a, b) (*(a)=xdigit[((b)>>4)&15],(a)[1]=xdigit[(b)&15]);

static char listline[LIST_MAX_LEN];
static bool listlinep;

static char listerror[LIST_MAX_LEN];

static char listdata[2 * LIST_INDENT];  /* we need less than that actually */
static int32_t listoffset;

static int32_t listlineno;

static int32_t listp;

static int suppress;            /* for INCBIN & TIMES special cases */

static int listlevel, listlevel_e;

static FILE *listfp;

static void list_emit(void) {
    int i;

    if (!listlinep && !listdata[0])
        return;

    fprintf(listfp, "%6"
    PRId32
    " ", ++listlineno);

    if (listdata[0])
        fprintf(listfp, "%08"
    PRIX32
    " %-*s", listoffset, LIST_HEXBIT + 1,
            listdata);
    else
    fprintf(listfp, "%*s", LIST_HEXBIT + 10, "");

    if (listlevel_e)
        fprintf(listfp, "%s<%d>", (listlevel < 10 ? " " : ""),
                listlevel_e);
    else if (listlinep)
        fprintf(listfp, "    ");

    if (listlinep)
        fprintf(listfp, " %s", listline);

    putc('\n', listfp);
    listlinep = false;
    listdata[0] = '\0';

    if (listerror[0]) {
        fprintf(listfp, "%6"
        PRId32
        "          ", ++listlineno);
        for (i = 0; i < LIST_HEXBIT; i++)
            putc('*', listfp);

        if (listlevel_e)
            fprintf(listfp, " %s<%d>", (listlevel < 10 ? " " : ""),
                    listlevel_e);
        else
            fprintf(listfp, "     ");

        fprintf(listfp, "  %s\n", listerror);
        listerror[0] = '\0';
    }
}

static void list_init(char *fname, efunc error) {
    listfp = fopen(fname, "w");
    if (!listfp) {
        error(ERR_NONFATAL, "unable to open listing file `%s'",
              fname);
        return;
    }

    *listline = '\0';
    listlineno = 0;
    *listerror = '\0';
    listp = true;
    listlevel = 0;
    suppress = 0;
    mistack = as_malloc(sizeof(MacroInhibit));
    mistack->next = NULL;
    mistack->level = 0;
    mistack->inhibiting = true;
}

static void list_cleanup(void) {
    if (!listp)
        return;

    while (mistack) {
        MacroInhibit *temp = mistack;
        mistack = temp->next;
        as_free(temp);
    }

    list_emit();
    fclose(listfp);
}

static void list_out(int32_t offset, char *str) {
    if (strlen(listdata) + strlen(str) > LIST_HEXBIT) {
        strcat(listdata, "-");
        list_emit();
    }
    if (!listdata[0])
        listoffset = offset;
    strcat(listdata, str);
}

static void list_address(int32_t offset, const char *brackets,
                         int64_t addr, int size) {
    char q[20];
    char *r = q;

    as_assert(size <= 8);

    *r++ = brackets[0];
    while (size--) {
        HEX(r, addr)
        addr >>= 8;
        r += 2;
    }
    *r++ = brackets[1];
    *r = '\0';
    list_out(offset, q);
}

static void list_output(int32_t offset, const void *data,
                        enum out_type type, uint64_t size) {
    char q[20];

    if (!listp || suppress || user_nolist)      /* fbk - 9/2/00 */
        return;

    switch (type) {
        case OUT_RAWDATA: {
            uint8_t const *p = data;

            if (size == 0 && !listdata[0])
                listoffset = offset;
            while (size--) {
                HEX(q, *p)
                q[2] = '\0';
                list_out(offset++, q);
                p++;
            }
            break;
        }
        case OUT_ADDRESS:
            list_address(offset, "[]", *(int64_t *) data, size);
            break;
        case OUT_REL1ADR:
            list_address(offset, "()", *(int64_t *) data, 1);
            break;
        case OUT_REL2ADR:
            list_address(offset, "()", *(int64_t *) data, 2);
            break;
        case OUT_REL4ADR:
            list_address(offset, "()", *(int64_t *) data, 4);
            break;
        case OUT_REL8ADR:
            list_address(offset, "()", *(int64_t *) data, 8);
            break;
        case OUT_RESERVE: {
            snprintf(q, sizeof(q), "<res %08"
            PRIX64
            ">", size);
            list_out(offset, q);
            break;
        }
    }
}

static void list_line(int type, char *line) {
    if (!listp)
        return;
    if (user_nolist) {          /* fbk - 9/2/00 */
        listlineno++;
        return;
    }

    if (mistack && mistack->inhibiting) {
        if (type == LIST_MACRO)
            return;
        else {                  /* pop the m i stack */
            MacroInhibit *temp = mistack;
            mistack = temp->next;
            as_free(temp);
        }
    }
    list_emit();
    listlinep = true;
    strncpy(listline, line, LIST_MAX_LEN - 1);
    listline[LIST_MAX_LEN - 1] = '\0';
    listlevel_e = listlevel;
}

static void list_uplevel(int type) {
    if (!listp)
        return;
    if (type == LIST_INCBIN || type == LIST_TIMES) {
        suppress |= (type == LIST_INCBIN ? 1 : 2);
        list_out(listoffset, type == LIST_INCBIN ? "<incbin>" : "<rept>");
        return;
    }

    listlevel++;

    if (mistack && mistack->inhibiting && type == LIST_INCLUDE) {
        MacroInhibit *temp = as_malloc(sizeof(MacroInhibit));
        temp->next = mistack;
        temp->level = listlevel;
        temp->inhibiting = false;
        mistack = temp;
    } else if (type == LIST_MACRO_NOLIST) {
        MacroInhibit *temp = as_malloc(sizeof(MacroInhibit));
        temp->next = mistack;
        temp->level = listlevel;
        temp->inhibiting = true;
        mistack = temp;
    }
}

static void list_downlevel(int type) {
    if (!listp)
        return;

    if (type == LIST_INCBIN || type == LIST_TIMES) {
        suppress &= ~(type == LIST_INCBIN ? 1 : 2);
        return;
    }

    listlevel--;
    while (mistack && mistack->level > listlevel) {
        MacroInhibit *temp = mistack;
        mistack = temp->next;
        as_free(temp);
    }
}

static void list_error(int severity, const char *pfx, const char *msg) {
    if (!listfp)
        return;

    snprintf(listerror, sizeof listerror, "%s%s", pfx, msg);

    if ((severity & ERR_MASK) >= ERR_FATAL)
        list_emit();
}


ListGen aslist = {
        list_init,
        list_cleanup,
        list_output,
        list_line,
        list_uplevel,
        list_downlevel,
        list_error
};
