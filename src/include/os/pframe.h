//
// Page frame database routines
//
#ifndef PFRAME_H
#define PFRAME_H

#define DMA_BUFFER_START 0x10000
#define DMA_BUFFER_PAGES 16

struct pageframe {
    unsigned long tag;
    union {
        unsigned long locks;        // Number of locks
        unsigned long size;         // Size/buckets for kernel pages
        handle_t owner;             // Reference to owner for file maps
        struct pageframe *next;     // Next free page frame for free pages
    };
};

extern struct pageframe *pfdb;

extern unsigned long freemem;
extern unsigned long totalmem;
extern unsigned long maxmem;

krnlapi unsigned long alloc_pageframe(unsigned long tag);

krnlapi unsigned long alloc_linear_pageframes(int pages, unsigned long tag);

krnlapi void free_pageframe(unsigned long pfn);

krnlapi void set_pageframe_tag(void *addr, unsigned int len, unsigned long tag);

void tag2str(unsigned long tag, char *str);

int memmap_proc(struct proc_file *pf, void *arg);

int memusage_proc(struct proc_file *pf, void *arg);

int memstat_proc(struct proc_file *pf, void *arg);

int physmem_proc(struct proc_file *pf, void *arg);

void init_pfdb();

#endif
