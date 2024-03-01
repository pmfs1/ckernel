//
// Routines for working with a resource map
//
#ifndef RMAP_H
#define RMAP_H

struct rmap {
    unsigned int offset;
    unsigned int size;
};

#ifdef  __cplusplus
extern "C" {
#endif

void rmap_init(struct rmap *r, unsigned int size);

unsigned int rmap_alloc(struct rmap *rmap, unsigned int size);

unsigned int rmap_alloc_align(struct rmap *rmap, unsigned int size, unsigned int align);

void rmap_free(struct rmap *rmap, unsigned int offset, unsigned int size);

int rmap_reserve(struct rmap *rmap, unsigned int offset, unsigned int size);

int rmap_status(struct rmap *rmap, unsigned int offset, unsigned int size);

#ifdef  __cplusplus
}
#endif

#endif
