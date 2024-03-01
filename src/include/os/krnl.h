//
// Main kernel include file
//
#ifndef KRNL_H
#define KRNL_H

#include <os.h>

#include <sys/types.h>
#include <stddef.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include <bitops.h>
#include <rmap.h>
#include <inifile.h>
#include <moddb.h>
#include <verinfo.h>

#include <os/tss.h>
#include <os/seg.h>
#include <os/fpu.h>
#include <os/cpu.h>

#include <os/pdir.h>
#include <os/pframe.h>

#include <os/mach.h>

#include <os/kmem.h>
#include <os/kmalloc.h>
#include <os/vmm.h>

#include <os/syspage.h>

#include <os/pe.h>

#include <os/buf.h>

#include <os/timer.h>
#include <os/user.h>
#include <os/object.h>
#include <os/queue.h>
#include <os/sched.h>
#include <os/trap.h>
#include <os/dbg.h>
#include <os/klog.h>

#include <os/pic.h>
#include <os/pit.h>

#include <os/dev.h>
#include <os/pci.h>
#include <os/pnpbios.h>
#include <os/virtio.h>

#include <os/video.h>
#include <os/kbd.h>
#include <os/rnd.h>

#include <os/iovec.h>
#include <os/vfs.h>
#include <os/dfs.h>
#include <os/devfs.h>
#include <os/procfs.h>

#include <os/mbr.h>

#include <os/pe.h>
#include <os/ldr.h>

#include <os/syscall.h>

#include <net/net.h>

#if _MSC_VER < 1300
#pragma warning(disable : 4761)
#endif

// start.c

krnlapi extern dev_t
bootdev;
krnlapi extern char krnlopts[KRNLOPTS_LEN];
krnlapi extern struct section *krnlcfg;
krnlapi extern struct peb *peb;

krnlapi void panic(char *msg);

krnlapi int license();

krnlapi void stop(int mode);

// syscall.c

void init_syscall();

// cpu.c

int cpu_proc(struct proc_file *pf, void *arg);

// smbfs.c

void init_smbfs();

// pipefs.c

void init_pipefs();

int pipe(struct file **readpipe, struct file **writepipe);

// cdfs.c

void init_cdfs();

// cons.c

extern int serial_console;

void init_console();

void console_print(char *buffer, int size);

// serial.c

void init_serial();

// ramdisk.c

int create_initrd();

// hd.c

void init_hd();

// fd.c

void init_fd();

// virtioblk.c

void init_vblk();

// apm.c

void apm_power_off();

extern int apm_enabled;

// opts.c

char *get_option(char *opts, char *name, char *buffer, int size, char *defval);

int get_num_option(char *opts, char *name, int defval);

// strtol.c

unsigned long strtoul(const char *nptr, char **endptr, int ibase);

// vsprintf.c

int vsprintf(char *buf, const char *fmt, va_list args);

int sprintf(char *buf, const char *fmt, ...);

#endif
