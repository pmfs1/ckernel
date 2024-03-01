/* 
 * labels.h  header file for labels.c
 */

#ifndef LABELS_H
#define LABELS_H

extern char lprefix[PREFIX_MAX];
extern char lpostfix[PREFIX_MAX];

bool lookup_label(char *label, int32_t *segment, int64_t *offset);

bool is_extern(char *label);

void define_label(char *label, int32_t segment, int64_t offset, char *special,
                  bool is_norm, bool isextrn);

void redefine_label(char *label, int32_t segment, int64_t offset, char *special,
                    bool is_norm, bool isextrn);

void define_common(char *label, int32_t segment, int32_t size, char *special);

void declare_as_global(char *label, char *special);

int init_labels(void);

void cleanup_labels(void);

char *local_scope(char *label);

#endif /* LABELS_H */
