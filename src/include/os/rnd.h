//
// A strong random number generator
//
#ifndef RND_H
#define RND_H

krnlapi void get_random_bytes(void *buf, int nbytes);

void add_dpc_randomness(void *dpc);

#endif
