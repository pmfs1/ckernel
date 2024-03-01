//
// Virtual memory manager
//
#ifndef VMM_H
#define VMM_H

extern struct rmap *vmap;

void init_vmm();

krnlapi void *vmalloc(void *addr, unsigned long size, int type, int protect, unsigned long tag, int *rc);

krnlapi void *vmmap(void *addr, unsigned long size, int protect, struct file *filp, off64_t offset, int *rc);

krnlapi int vmsync(void *addr, unsigned long size);

krnlapi int vmfree(void *addr, unsigned long size, int type);

krnlapi void *
vmrealloc(void *addr, unsigned long oldsize, unsigned long newsize, int type, int protect, unsigned long tag);

krnlapi int vmprotect(void *addr, unsigned long size, int protect);

krnlapi int vmlock(void *addr, unsigned long size);

krnlapi int vmunlock(void *addr, unsigned long size);

krnlapi void *miomap(unsigned long physaddr, int size, int protect);

krnlapi void miounmap(void *addr, int size);

int guard_page_handler(void *addr);

int fetch_page(void *addr);

int vmem_proc(struct proc_file *pf, void *arg);

int mem_sysinfo(struct meminfo *info);

#endif
