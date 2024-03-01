/* 
 * eval.h   header file for eval.c
 */

#ifndef AS_EVAL_H
#define AS_EVAL_H

/*
 * Called once to tell the evaluator what output format is
 * providing segment-base details, and what function can be used to
 * look labels up.
 */
void eval_global_info(struct ofmt *output, lfunc lookup_label,
                      struct location *locp);

/*
 * The evaluator itself.
 */
expr *evaluate(scanner sc, void *scprivate, struct tokenval *tv,
               int *fwref, int critical, efunc report_error,
               struct eval_hints *hints);

void eval_cleanup(void);

#endif
