//
// Stacked memory allocation
//
#ifndef STMALLOC_H
#define STMALLOC_H

#define STKBLKMIN (4096 - 8 - 8)

struct stkblk {
    struct stkblk *prev;
    char space[STKBLKMIN];
};

struct stkmark {
    struct stkblk *blk;
    char *ptr;
    char *txt;
    char *end;
    struct stkmark *prev;
};

void pushstkmark(struct stkmark *oldmark, struct stkmark *newmark);

void popstkmark(struct stkmark *mark);

void *stalloc(struct stkmark *mark, int size);

int stputbuf(struct stkmark *mark, char *data, int len);

int stputc(struct stkmark *mark, int ch);

int stputstr(struct stkmark *mark, char *str);

char *ststrdup(struct stkmark *mark, char *str);

char *ststr(struct stkmark *mark);

char *ststrptr(struct stkmark *mark);

int ststrlen(struct stkmark *mark);

int stfreestr(struct stkmark *mark, char *str);

#endif
