/* 
 * stdscan.h	header file for stdscan.c
 */

#ifndef AS_STDSCAN_H
#define AS_STDSCAN_H

/* Standard scanner */
void stdscan_set(char *str);

char *stdscan_get(void);

void stdscan_reset(void);

int stdscan(void *private_data, struct tokenval *tv);

int as_token_hash(const char *token, struct tokenval *tv);

void stdscan_cleanup(void);

#endif
