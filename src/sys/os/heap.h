//
// Heap memory management routines
//
#ifndef HEAP_H
#define HEAP_H

struct heap;

void *heap_alloc(struct heap *av, size_t size);

void *heap_realloc(struct heap *av, void *mem, size_t size);

void *heap_calloc(struct heap *av, size_t num, size_t size);

void heap_free(struct heap *av, void *p);

struct mallinfo heap_mallinfo(struct heap *av);

int heap_malloc_usable_size(void *p);

struct heap *heap_create(size_t region_size, size_t group_size);

int heap_destroy(struct heap *av);

#endif
