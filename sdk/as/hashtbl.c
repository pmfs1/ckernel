/*
 * hashtbl.c
 *
 * Efficient dictionary hash table class.
 */

#include "compiler.h"

#include <inttypes.h>
#include <string.h>
#include "as.h"
#include "hashtbl.h"

#define HASH_MAX_LOAD   2 /* Higher = more memory-efficient, slower */

#define hash_calc(key)          crc64(CRC64_INIT, (key))
#define hash_calci(key)         crc64i(CRC64_INIT, (key))
#define hash_max_load(size)     ((size) * (HASH_MAX_LOAD - 1) / HASH_MAX_LOAD)
#define hash_expand(size)       ((size) << 1)
#define hash_mask(size)         ((size) - 1)
#define hash_pos(hash, mask)    ((hash) & (mask))
#define hash_inc(hash, mask)    ((((hash) >> 32) & (mask)) | 1) /* always odd */
#define hash_pos_next(pos, inc, mask) (((pos) + (inc)) & (mask))

static struct hash_tbl_node *alloc_table(size_t newsize) {
    size_t bytes = newsize * sizeof(struct hash_tbl_node);
    return as_zalloc(bytes);
}

void hash_init(struct hash_table *head, size_t size) {
    as_assert(is_power2(size));
    head->table = alloc_table(size);
    head->load = 0;
    head->size = size;
    head->max_load = hash_max_load(size);
}

/*
 * Find an entry in a hash table.
 *
 * On failure, if "insert" is non-NULL, store data in that structure
 * which can be used to insert that node using hash_add().
 *
 * WARNING: this data is only valid until the very next call of
 * hash_add(); it cannot be "saved" to a later date.
 *
 * On success, return a pointer to the "data" element of the hash
 * structure.
 */
void **hash_find(struct hash_table *head, const char *key,
                 struct hash_insert *insert) {
    struct hash_tbl_node *np;
    struct hash_tbl_node *tbl = head->table;
    uint64_t hash = hash_calc(key);
    size_t mask = hash_mask(head->size);
    size_t pos = hash_pos(hash, mask);
    size_t inc = hash_inc(hash, mask);

    while ((np = &tbl[pos])->key) {
        if (hash == np->hash && !strcmp(key, np->key))
            return &np->data;
        pos = hash_pos_next(pos, inc, mask);
    }

    /* Not found.  Store info for insert if requested. */
    if (insert) {
        insert->head = head;
        insert->hash = hash;
        insert->where = np;
    }
    return NULL;
}

/*
 * Same as hash_find, but for case-insensitive hashing.
 */
void **hash_findi(struct hash_table *head, const char *key,
                  struct hash_insert *insert) {
    struct hash_tbl_node *np;
    struct hash_tbl_node *tbl = head->table;
    uint64_t hash = hash_calci(key);
    size_t mask = hash_mask(head->size);
    size_t pos = hash_pos(hash, mask);
    size_t inc = hash_inc(hash, mask);

    while ((np = &tbl[pos])->key) {
        if (hash == np->hash && !as_stricmp(key, np->key))
            return &np->data;
        pos = hash_pos_next(pos, inc, mask);
    }

    /* Not found.  Store info for insert if requested. */
    if (insert) {
        insert->head = head;
        insert->hash = hash;
        insert->where = np;
    }
    return NULL;
}

/*
 * Insert node.  Return a pointer to the "data" element of the newly
 * created hash node.
 */
void **hash_add(struct hash_insert *insert, const char *key, void *data) {
    struct hash_table *head = insert->head;
    struct hash_tbl_node *np = insert->where;

    /*
     * Insert node.  We can always do this, even if we need to
     * rebalance immediately after.
     */
    np->hash = insert->hash;
    np->key = key;
    np->data = data;

    if (++head->load > head->max_load) {
        /* Need to expand the table */
        size_t newsize = hash_expand(head->size);
        struct hash_tbl_node *newtbl = alloc_table(newsize);
        size_t mask = hash_mask(newsize);

        if (head->table) {
            struct hash_tbl_node *op, *xp;
            size_t i;

            /* Rebalance all the entries */
            for (i = 0, op = head->table; i < head->size; i++, op++) {
                if (op->key) {
                    size_t pos = hash_pos(op->hash, mask);
                    size_t inc = hash_inc(op->hash, mask);

                    while ((xp = &newtbl[pos])->key)
                        pos = hash_pos_next(pos, inc, mask);

                    *xp = *op;
                    if (op == np)
                        np = xp;
                }
            }
            as_free(head->table);
        }

        head->table = newtbl;
        head->size = newsize;
        head->max_load = hash_max_load(newsize);
    }

    return &np->data;
}

/*
 * Iterate over all members of a hash set.  For the first call,
 * iterator should be initialized to NULL.  Returns the data pointer,
 * or NULL on failure.
 */
void *hash_iterate(const struct hash_table *head,
                   struct hash_tbl_node **iterator,
                   const char **key) {
    struct hash_tbl_node *np = *iterator;
    struct hash_tbl_node *ep = head->table + head->size;

    if (!np) {
        np = head->table;
        if (!np)
            return NULL;        /* Uninitialized table */
    }

    while (np < ep) {
        if (np->key) {
            *iterator = np + 1;
            if (key)
                *key = np->key;
            return np->data;
        }
        np++;
    }

    *iterator = NULL;
    if (key)
        *key = NULL;
    return NULL;
}

/*
 * Free the hash itself.  Doesn't free the data elements; use
 * hash_iterate() to do that first, if needed.
 */
void hash_free(struct hash_table *head) {
    void *p = head->table;
    head->table = NULL;
    as_free(p);
}
