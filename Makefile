CC=bin/utils/cc
AR=bin/utils/ar
AS=bin/utils/as
MKDFS=bin/utils/mkdfs
MKPKG=bin/utils/mkpkg

OBJ=bin/obj

LIB_KRNL=bin/install/usr/lib/krnl.def
LIB_OS=bin/install/usr/lib/os.def

CCFLAGS=-B bin/install/usr

PKGREPO=/mnt/pkg
PKGDIR=bin/install/usr/share/pkg

ckernel: dirs build-utils boot sys libc utils cmds dev
	cp -p ckernel.inf $(PKGDIR)

dirs: bin/.timestamp

bin/.timestamp:
	mkdir -p bin/
	mkdir -p bin/utils
	mkdir -p bin/obj
	mkdir -p bin/img
	mkdir -p bin/install
	mkdir -p bin/install/bin
	mkdir -p bin/install/boot
	mkdir -p bin/install/dev
	mkdir -p bin/install/etc
	mkdir -p bin/install/mnt
	mkdir -p bin/install/proc
	mkdir -p bin/install/tmp
	mkdir -p bin/install/usr
	mkdir -p bin/install/usr/bin
	mkdir -p bin/install/usr/lib
	mkdir -p bin/install/usr/src
	mkdir -p bin/install/usr/src/utils
	mkdir -p bin/install/usr/share
	mkdir -p bin/install/usr/share/pkg
	mkdir -p bin/install/var
	[ -d bin/install/boot/krnl.ini ]         || ln -s ../../../krnl.ini bin/install/boot/krnl.ini
	[ -d bin/install/etc/os.ini ]            || ln -s ../../../os.ini bin/install/etc/os.ini
	[ -d bin/install/etc/setup.ini ]         || ln -s ../../../setup.ini bin/install/etc/setup.ini
	[ -d bin/install/usr/include ]           || ln -s ../../../src/include bin/install/usr/include
	[ -d bin/install/usr/src/lib ]           || ln -s ../../../../src/lib bin/install/usr/src/lib
	[ -d bin/install/usr/src/sys ]           || ln -s ../../../../src/sys bin/install/usr/src/sys
	[ -d bin/install/usr/src/cmds ]          || ln -s ../../../../src/cmds bin/install/usr/src/cmds
	[ -d bin/install/usr/src/utils/ar ]      || ln -s ../../../../../src/utils/ar bin/install/usr/src/utils/ar
	[ -d bin/install/usr/src/utils/edit ]    || ln -s ../../../../../src/utils/edit bin/install/usr/src/utils/edit
	[ -d bin/install/usr/src/utils/fdisk ]   || ln -s ../../../../../src/utils/fdisk bin/install/usr/src/utils/fdisk
	[ -d bin/install/usr/src/utils/ftpd ]    || ln -s ../../../../../src/utils/ftpd bin/install/usr/src/utils/ftpd
	[ -d bin/install/usr/src/utils/genvmdk ] || ln -s ../../../../../src/utils/genvmdk bin/install/usr/src/utils/genvmdk
	[ -d bin/install/usr/src/utils/httpd ]   || ln -s ../../../../../src/utils/httpd bin/install/usr/src/utils/httpd
	[ -d bin/install/usr/src/utils/impdef ]  || ln -s ../../../../../src/utils/impdef bin/install/usr/src/utils/impdef
	[ -d bin/install/usr/src/utils/login ]   || ln -s ../../../../../src/utils/login bin/install/usr/src/utils/login
	[ -d bin/install/usr/src/utils/make ]    || ln -s ../../../../../src/utils/make bin/install/usr/src/utils/make
	[ -d bin/install/usr/src/utils/mkboot ]  || ln -s ../../../../../src/utils/mkboot bin/install/usr/src/utils/mkboot
	[ -d bin/install/usr/src/utils/msh ]     || ln -s ../../../../../src/utils/msh bin/install/usr/src/utils/msh
	[ -d bin/install/usr/src/utils/setup ]   || ln -s ../../../../../src/utils/setup bin/install/usr/src/utils/setup
	[ -d bin/install/usr/src/utils/sh ]      || ln -s ../../../../../src/utils/sh bin/install/usr/src/utils/sh
	[ -d bin/install/usr/src/utils/telnetd ] || ln -s ../../../../../src/utils/telnetd bin/install/usr/src/utils/telnetd
	[ -d bin/install/usr/src/utils/cc ]      || ln -s ../../../../../sdk/cc bin/install/usr/src/utils/cc
	[ -d bin/install/usr/src/utils/as ]      || ln -s ../../../../../sdk/as bin/install/usr/src/utils/as
	[ -d bin/install/usr/src/win32 ]         || ln -s ../../../../src/win32 bin/install/usr/src/win32
	[ -d bin/install/usr/src/Makefile ]      || ln -s ../../../../src/Makefile bin/install/usr/src/Makefile
	touch bin/.timestamp

build-utils: $(CC) $(AS) $(AR) $(MKDFS) $(MKPKG)

GCC_FLAGS=-O2 -m32 -w

CC_SRCFILES=\
  sdk/cc/asm386.c \
  sdk/cc/asm.c \
  sdk/cc/cc.c \
  sdk/cc/codegen386.c \
  sdk/cc/codegen.c \
  sdk/cc/compiler.c \
  sdk/cc/elf.c \
  sdk/cc/pe.c \
  sdk/cc/preproc.c \
  sdk/cc/symbol.c \
  sdk/cc/type.c \
  sdk/cc/util.c

CC_HDRFILES=\
  sdk/cc/cc.h \
  sdk/cc/config.h \
  sdk/cc/elf.h \
  sdk/cc/opcodes.h \
  sdk/cc/tokens.h

$(CC): $(CC_SRCFILES) $(CC_HDRFILES)
	gcc $(GCC_FLAGS) -o $(CC) $(CC_SRCFILES) -I sdk/cc

AS_OUTDEFS=-DOF_ONLY -DOF_ELF32 -DOF_WIN32 -DOF_COFF -DOF_OBJ -DOF_BIN -DOF_DBG -DOF_DEFAULT=of_elf32
AS_CFLAGS=-I. -DHAVE_SNPRINTF -DHAVE_VSNPRINTF $(AS_OUTDEFS)

AS_SRCFILES= \
	sdk/as/as.c sdk/as/aslib.c sdk/as/ver.c sdk/as/raa.c sdk/as/saa.c \
	sdk/as/rbtree.c sdk/as/float.c sdk/as/insnsa.c sdk/as/insnsb.c sdk/as/directiv.c \
	sdk/as/assemble.c sdk/as/labels.c sdk/as/hashtbl.c sdk/as/crc64.c sdk/as/parser.c \
	sdk/as/preproc.c sdk/as/quote.c sdk/as/pptok.c sdk/as/macros.c sdk/as/listing.c \
	sdk/as/eval.c sdk/as/exprlib.c sdk/as/stdscan.c sdk/as/strfunc.c sdk/as/tokhash.c \
	sdk/as/regvals.c sdk/as/regflags.c sdk/as/ilog2.c sdk/as/strlcpy.c \
	sdk/as/out/outform.c sdk/as/out/outlib.c sdk/as/out/nulldbg.c sdk/as/out/nullout.c \
	sdk/as/out/outbin.c sdk/as/out/outcoff.c sdk/as/out/outelf.c sdk/as/out/outelf32.c \
	sdk/as/out/outobj.c sdk/as/out/outdbg.c

$(AS): $(AS_SRCFILES)
	gcc $(GCC_FLAGS) -o $(AS) $(AS_SRCFILES) $(AS_OUTDEFS) -I sdk/as

$(AR): src/utils/ar/ar.c
	gcc $(GCC_FLAGS) -o $(AR) src/utils/ar/ar.c -DO_BINARY=0

MKDFS_SRCFILES= \
	utils/dfs/blockdev.c utils/dfs/vmdk.c utils/dfs/bitops.c utils/dfs/buf.c utils/dfs/dfs.c \
	utils/dfs/dir.c utils/dfs/file.c utils/dfs/group.c utils/dfs/inode.c \
	utils/dfs/mkdfs.c utils/dfs/super.c utils/dfs/vfs.c

$(MKDFS): $(MKDFS_SRCFILES)
	gcc $(GCC_FLAGS) -o $(MKDFS) $(MKDFS_SRCFILES) -DO_BINARY=0

$(MKPKG): utils/mkpkg/mkpkg.c utils/mkpkg/inifile.c
	gcc $(GCC_FLAGS) -o $(MKPKG) utils/mkpkg/mkpkg.c utils/mkpkg/inifile.c

sys: osldr kernel drivers os
	cp -p sys.inf $(PKGDIR)

boot: bin/install/boot/boot bin/install/boot/cdboot bin/install/boot/cdemboot bin/install/boot/netboot

bin/install/boot/boot: src/sys/boot/boot.asm
	$(AS) -f bin $^ -o $@

bin/install/boot/cdboot: src/sys/boot/cdboot.asm
	$(AS) -f bin $^ -o $@

bin/install/boot/cdemboot: src/sys/boot/cdemboot.asm
	$(AS) -f bin $^ -o $@

bin/install/boot/netboot: src/sys/boot/netboot.asm
	$(AS) -f bin $^ -o $@

osldr: bin/install/boot/osldr.dll

$(OBJ)/bioscall.o: src/sys/osldr/bioscall.asm
	$(AS) $^ -o $@

$(OBJ)/ldrinit.exe: src/sys/osldr/ldrinit.asm
	$(AS) -f bin $^ -o $@

OSLDR_SRCS=src/sys/osldr/osldr.c src/sys/osldr/loadkrnl.c src/sys/osldr/unzip.c
OSLDR_LIBSRCS=src/lib/vsprintf.c src/lib/string.c

bin/install/boot/osldr.dll: $(OSLDR_SRCS) $(OSLDR_LIBSRCS) $(OBJ)/bioscall.o $(OBJ)/ldrinit.exe
	$(CC) -shared -entry _start@12 -fixed 0x00090000 -filealign 4096 -stub $(OBJ)/ldrinit.exe -nostdlib -o $@ $(OSLDR_SRCS) $(OSLDR_LIBSRCS) $(OBJ)/bioscall.o -D OSLDR -D KERNEL $(CCFLAGS)

kernel: bin/install/boot/krnl.dll

KRNL_SRCS=\
  src/sys/krnl/apm.c \
  src/sys/krnl/buf.c \
  src/sys/krnl/cpu.c \
  src/sys/krnl/dbg.c \
  src/sys/krnl/dev.c \
  src/sys/krnl/fpu.c \
  src/sys/krnl/hndl.c \
  src/sys/krnl/iomux.c \
  src/sys/krnl/iop.c \
  src/sys/krnl/iovec.c \
  src/sys/krnl/kmalloc.c \
  src/sys/krnl/kmem.c \
  src/sys/krnl/ldr.c \
  src/sys/krnl/mach.c \
  src/sys/krnl/object.c \
  src/sys/krnl/pci.c \
  src/sys/krnl/pdir.c \
  src/sys/krnl/pframe.c \
  src/sys/krnl/pic.c \
  src/sys/krnl/pit.c \
  src/sys/krnl/pnpbios.c \
  src/sys/krnl/queue.c \
  src/sys/krnl/sched.c \
  src/sys/krnl/start.c \
  src/sys/krnl/syscall.c \
  src/sys/krnl/timer.c \
  src/sys/krnl/trap.c \
  src/sys/krnl/user.c \
  src/sys/krnl/vfs.c \
  src/sys/krnl/virtio.c \
  src/sys/krnl/vmi.c \
  src/sys/krnl/vmm.c

DEV_SRCS=\
  src/sys/dev/cons.c \
  src/sys/dev/fd.c \
  src/sys/dev/hd.c \
  src/sys/dev/kbd.c \
  src/sys/dev/klog.c \
  src/sys/dev/null.c \
  src/sys/dev/nvram.c \
  src/sys/dev/ramdisk.c \
  src/sys/dev/rnd.c \
  src/sys/dev/serial.c \
  src/sys/dev/smbios.c \
  src/sys/dev/vga.c \
  src/sys/dev/video.c \
  src/sys/dev/virtioblk.c \
  src/sys/dev/virtiocon.c

NET_SRCS=\
  src/sys/net/arp.c \
  src/sys/net/dhcp.c \
  src/sys/net/ether.c \
  src/sys/net/icmp.c \
  src/sys/net/inet.c \
  src/sys/net/ipaddr.c \
  src/sys/net/ip.c \
  src/sys/net/loopif.c \
  src/sys/net/netif.c \
  src/sys/net/pbuf.c \
  src/sys/net/raw.c \
  src/sys/net/rawsock.c \
  src/sys/net/socket.c \
  src/sys/net/stats.c \
  src/sys/net/tcp.c \
  src/sys/net/tcp_input.c \
  src/sys/net/tcp_output.c \
  src/sys/net/tcpsock.c \
  src/sys/net/udp.c \
  src/sys/net/udpsock.c \

FS_SRCS=\
  src/sys/fs/cdfs/cdfs.c \
  src/sys/fs/devfs/devfs.c \
  src/sys/fs/dfs/dfs.c \
  src/sys/fs/dfs/dir.c \
  src/sys/fs/dfs/file.c \
  src/sys/fs/dfs/group.c \
  src/sys/fs/dfs/inode.c \
  src/sys/fs/dfs/super.c \
  src/sys/fs/pipefs/pipefs.c \
  src/sys/fs/procfs/procfs.c \
  src/sys/fs/smbfs/smbcache.c \
  src/sys/fs/smbfs/smbfs.c \
  src/sys/fs/smbfs/smbproto.c \
  src/sys/fs/smbfs/smbutil.c

KRNL_LIB_SRCS=\
  src/lib/bitops.c \
  src/lib/ctype.c \
  src/lib/inifile.c \
  src/lib/moddb.c \
  src/lib/opts.c \
  src/lib/rmap.c \
  src/lib/string.c \
  src/lib/strtol.c \
  src/lib/ccrt.c \
  src/lib/time.c \
  src/lib/verinfo.c \
  src/lib/vsprintf.c

KERNEL_SRCS=$(KRNL_SRCS) $(DEV_SRCS) $(NET_SRCS) $(FS_SRCS) $(KRNL_LIB_SRCS)

bin/install/boot/krnl.dll $(LIB_KRNL): $(KERNEL_SRCS)
	$(CC) -shared -entry _start@12 -fixed 0x80000000 -filealign 4096 -nostdlib -o bin/install/boot/krnl.dll -def $(LIB_KRNL) -D KERNEL -D KRNL_LIB $(KERNEL_SRCS) $(CCFLAGS)

drivers: \
  bin/install/boot/3c905c.sys \
  bin/install/boot/eepro100.sys \
  bin/install/boot/ne2000.sys \
  bin/install/boot/pcnet32.sys \
  bin/install/boot/rtl8139.sys \
  bin/install/boot/sis900.sys \
  bin/install/boot/tulip.sys \
  bin/install/boot/virtionet.sys

bin/install/boot/3c905c.sys: src/sys/dev/3c905c.c src/lib/string.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

bin/install/boot/eepro100.sys: src/sys/dev/eepro100.c src/lib/opts.c src/lib/string.c src/lib/strtol.c src/lib/ctype.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

bin/install/boot/ne2000.sys: src/sys/dev/ne2000.c src/lib/opts.c src/lib/string.c src/lib/strtol.c src/lib/ctype.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

bin/install/boot/pcnet32.sys: src/sys/dev/pcnet32.c src/lib/string.c src/lib/ctype.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

bin/install/boot/rtl8139.sys: src/sys/dev/rtl8139.c src/lib/opts.c src/lib/string.c src/lib/strtol.c src/lib/ctype.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

bin/install/boot/sis900.sys: src/sys/dev/sis900.c src/lib/opts.c src/lib/string.c src/lib/strtol.c src/lib/ctype.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

bin/install/boot/tulip.sys: src/sys/dev/tulip.c src/lib/opts.c src/lib/string.c src/lib/strtol.c src/lib/ctype.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

bin/install/boot/virtionet.sys: src/sys/dev/virtionet.c src/lib/string.c
	$(CC) -shared -entry _start@12 -nostdlib -lkrnl -o $@ -D KERNEL $^ $(CCFLAGS)

os: bin/install/boot/os.dll

OS_SRCS= \
  src/sys/os/critsect.c \
  src/sys/os/environ.c \
  src/sys/os/heap.c \
  src/sys/os/netdb.c \
  src/sys/os/os.c \
  src/sys/os/resolv.c \
  src/sys/os/signal.c \
  src/sys/os/sntp.c \
  src/sys/os/sysapi.c \
  src/sys/os/syserr.c \
  src/sys/os/syslog.c \
  src/sys/os/thread.c \
  src/sys/os/tls.c \
  src/sys/os/userdb.c

OS_LIB_SRCS= \
  src/lib/bitops.c \
  src/lib/crypt.c \
  src/lib/ctype.c \
  src/lib/fcvt.c \
  src/lib/inifile.c \
  src/lib/moddb.c \
  src/lib/opts.c \
  src/lib/strftime.c \
  src/lib/string.c \
  src/lib/strtol.c \
  src/lib/ccrt.c \
  src/lib/time.c \
  src/lib/verinfo.c \
  src/lib/vsprintf.c

bin/install/boot/os.dll $(LIB_OS): $(OS_SRCS) $(OS_LIB_SRCS) $(OBJ)/modf.o
	$(CC) -shared -entry _start@12 -fixed 0x7FF00000 -nostdlib -o bin/install/boot/os.dll -def $(LIB_OS) $^ -D OS_LIB $(CCFLAGS)

libc: bin/install/usr/lib/libc.a
	cp -p clib.inf $(PKGDIR)

LIBC_OBJ = \
  $(OBJ)/ccrt.o \
  $(OBJ)/assert.o \
  $(OBJ)/bsearch.o \
  $(OBJ)/conio.o \
  $(OBJ)/crt0.o \
  $(OBJ)/ctype.o \
  $(OBJ)/dirent.o \
  $(OBJ)/fcvt.o \
  $(OBJ)/fnmatch.o \
  $(OBJ)/fork.o \
  $(OBJ)/getopt.o \
  $(OBJ)/glob.o \
  $(OBJ)/hash.o \
  $(OBJ)/inifile.o \
  $(OBJ)/input.o \
  $(OBJ)/math.o \
  $(OBJ)/mman.o \
  $(OBJ)/opts.o \
  $(OBJ)/output.o \
  $(OBJ)/qsort.o \
  $(OBJ)/random.o \
  $(OBJ)/readline.o \
  $(OBJ)/rmap.o \
  $(OBJ)/rtttl.o \
  $(OBJ)/sched.o \
  $(OBJ)/semaphore.o \
  $(OBJ)/stdio.o \
  $(OBJ)/shlib.o \
  $(OBJ)/scanf.o \
  $(OBJ)/printf.o \
  $(OBJ)/tmpfile.o \
  $(OBJ)/popen.o \
  $(OBJ)/stdlib.o \
  $(OBJ)/strftime.o \
  $(OBJ)/string.o \
  $(OBJ)/strtod.o \
  $(OBJ)/strtol.o \
  $(OBJ)/termios.o \
  $(OBJ)/time.o \
  $(OBJ)/xtoa.o \
  $(OBJ)/regcomp.o \
  $(OBJ)/regexec.o \
  $(OBJ)/regerror.o \
  $(OBJ)/regfree.o \
  $(OBJ)/barrier.o \
  $(OBJ)/condvar.o \
  $(OBJ)/mutex.o \
  $(OBJ)/pthread.o \
  $(OBJ)/rwlock.o \
  $(OBJ)/spinlock.o \
  $(OBJ)/setjmp.o \
  $(OBJ)/chkstk.o

LIBC_MATHOBJ = \
  $(OBJ)/acos.o \
  $(OBJ)/asin.o \
  $(OBJ)/atan.o \
  $(OBJ)/atan2.o \
  $(OBJ)/ceil.o \
  $(OBJ)/cos.o \
  $(OBJ)/cosh.o \
  $(OBJ)/exp.o \
  $(OBJ)/fabs.o \
  $(OBJ)/floor.o \
  $(OBJ)/fmod.o \
  $(OBJ)/fpconst.o \
  $(OBJ)/fpreset.o \
  $(OBJ)/frexp.o \
  $(OBJ)/ftol.o \
  $(OBJ)/ldexp.o \
  $(OBJ)/log.o \
  $(OBJ)/log10.o \
  $(OBJ)/modf.o \
  $(OBJ)/pow.o \
  $(OBJ)/sin.o \
  $(OBJ)/sinh.o \
  $(OBJ)/sqrt.o \
  $(OBJ)/tan.o \
  $(OBJ)/tanh.o

CCLIBFLAGS=-I src/include -g
ASLIBFLAGS=

$(OBJ)/ccrt.o: src/lib/ccrt.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/assert.o: src/lib/assert.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/bsearch.o: src/lib/bsearch.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/conio.o: src/lib/conio.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/crt0.o: src/lib/crt0.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/ctype.o: src/lib/ctype.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/dirent.o: src/lib/dirent.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/fcvt.o: src/lib/fcvt.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/fnmatch.o: src/lib/fnmatch.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/fork.o: src/lib/fork.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/getopt.o: src/lib/getopt.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/glob.o: src/lib/glob.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/hash.o: src/lib/hash.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/inifile.o: src/lib/inifile.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/input.o: src/lib/input.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/math.o: src/lib/math.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/mman.o: src/lib/mman.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/opts.o: src/lib/opts.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/output.o: src/lib/output.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/qsort.o: src/lib/qsort.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/random.o: src/lib/random.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/readline.o: src/lib/readline.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/rmap.o: src/lib/rmap.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/rtttl.o: src/lib/rtttl.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/sched.o: src/lib/sched.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/semaphore.o: src/lib/semaphore.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/stdio.o: src/lib/stdio.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/shlib.o: src/lib/shlib.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/printf.o: src/lib/printf.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/scanf.o: src/lib/scanf.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/tmpfile.o: src/lib/tmpfile.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/popen.o: src/lib/popen.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/setjmp.o: src/lib/setjmp.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/stdlib.o: src/lib/stdlib.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/strftime.o: src/lib/strftime.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/string.o: src/lib/string.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/strtod.o: src/lib/strtod.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/strtol.o: src/lib/strtol.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/termios.o: src/lib/termios.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/time.o: src/lib/time.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/xtoa.o: src/lib/xtoa.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/regcomp.o: src/lib/regex/regcomp.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/regexec.o: src/lib/regex/regexec.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/regerror.o: src/lib/regex/regerror.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/regfree.o: src/lib/regex/regfree.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/barrier.o: src/lib/pthread/barrier.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/condvar.o: src/lib/pthread/condvar.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/mutex.o: src/lib/pthread/mutex.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/pthread.o: src/lib/pthread/pthread.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/rwlock.o: src/lib/pthread/rwlock.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/spinlock.o: src/lib/pthread/spinlock.c
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/chkstk.o: src/lib/chkstk.s
	$(CC) -o $@ -c $^ $(CCLIBFLAGS)

$(OBJ)/acos.o: src/lib/math/acos.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/asin.o: src/lib/math/asin.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/atan.o: src/lib/math/atan.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/atan2.o: src/lib/math/atan2.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/ceil.o: src/lib/math/ceil.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/cos.o: src/lib/math/cos.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/cosh.o: src/lib/math/cosh.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/exp.o: src/lib/math/exp.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/fabs.o: src/lib/math/fabs.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/floor.o: src/lib/math/floor.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/fmod.o: src/lib/math/fmod.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/fpconst.o: src/lib/math/fpconst.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/fpreset.o: src/lib/math/fpreset.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/frexp.o: src/lib/math/frexp.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/ftol.o: src/lib/math/ftol.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/ldexp.o: src/lib/math/ldexp.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/log.o: src/lib/math/log.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/log10.o: src/lib/math/log10.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/modf.o: src/lib/math/modf.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/pow.o: src/lib/math/pow.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/sin.o: src/lib/math/sin.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/sinh.o: src/lib/math/sinh.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/sqrt.o: src/lib/math/sqrt.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/tan.o: src/lib/math/tan.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

$(OBJ)/tanh.o: src/lib/math/tanh.asm
	$(AS) $^ -o $@ $(ASLIBFLAGS)

bin/install/usr/lib/libc.a: $(LIBC_OBJ) $(LIBC_MATHOBJ)
	$(AR) -s -m $@ $^

utils: \
  bin/install/bin/sh.exe \
  bin/install/bin/msh.exe \
  bin/install/bin/edit.exe \
  bin/install/bin/less.exe \
  bin/install/bin/fdisk.exe \
  bin/install/bin/setup.exe \
  bin/install/bin/mkboot.exe \
  bin/install/bin/genvmdk.exe \
  bin/install/bin/ftpd.exe \
  bin/install/bin/telnetd.exe \
  bin/install/bin/login.exe \
  bin/install/usr/bin/make.exe \
  bin/install/usr/bin/ar.exe \
  bin/install/usr/bin/impdef.exe

SH_SRCS=src/utils/sh/sh.c src/utils/sh/node.c src/utils/sh/job.c src/utils/sh/interp.c src/utils/sh/parser.c src/utils/sh/stmalloc.c src/utils/sh/input.c src/utils/sh/chartype.c src/utils/sh/cmds.c src/utils/sh/builtins.c
SH_HDRS=src/utils/sh/sh.h src/utils/sh/node.h src/utils/sh/job.h src/utils/sh/interp.h src/utils/sh/parser.h src/utils/sh/stmalloc.h src/utils/sh/input.h src/utils/sh/chartype.h
SH_CMDS= \
  src/cmds/chgrp.c \
  src/cmds/chmod.c \
  src/cmds/chown.c \
  src/cmds/cp.c \
  src/cmds/du.c \
  src/cmds/ls.c \
  src/cmds/mkdir.c \
  src/cmds/mv.c \
  src/cmds/rm.c \
  src/cmds/test.c \
  src/cmds/touch.c \
  src/cmds/wc.c

bin/install/bin/sh.exe: $(SH_SRCS) $(SH_HDRS) $(SH_CMDS)
	$(CC) -o $@ $(SH_SRCS) $(SH_CMDS) $(CCFLAGS) -D SHELL
	cp -p sh.inf $(PKGDIR)

bin/install/bin/msh.exe: src/utils/msh/msh.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p msh.inf $(PKGDIR)

bin/install/bin/edit.exe: src/utils/edit/edit.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p edit.inf $(PKGDIR)

bin/install/bin/less.exe: src/utils/edit/edit.c
	$(CC) -o $@ $^ $(CCFLAGS) -DLESS

bin/install/bin/fdisk.exe: src/utils/fdisk/fdisk.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p fdisk.inf $(PKGDIR)

bin/install/bin/setup.exe: src/utils/setup/setup.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p setup.inf $(PKGDIR)

bin/install/bin/mkboot.exe: src/utils/mkboot/mkboot.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p mkboot.inf $(PKGDIR)

bin/install/bin/genvmdk.exe: src/utils/genvmdk/genvmdk.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p genvmdk.inf $(PKGDIR)

bin/install/bin/ftpd.exe: src/utils/ftpd/ftpd.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p ftpd.inf $(PKGDIR)

bin/install/bin/telnetd.exe: src/utils/telnetd/telnetd.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p telnetd.inf $(PKGDIR)

bin/install/bin/login.exe: src/utils/login/login.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p login.inf $(PKGDIR)

bin/install/usr/bin/make.exe: src/utils/make/make.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p make.inf $(PKGDIR)

bin/install/usr/bin/ar.exe: src/utils/ar/ar.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p ar.inf $(PKGDIR)

bin/install/usr/bin/impdef.exe: src/utils/impdef/impdef.c
	$(CC) -o $@ $^ $(CCFLAGS)
	cp -p impdef.inf $(PKGDIR)

cmds: \
  bin/install/bin/find.exe \
  bin/install/bin/grep.exe \
  bin/install/bin/ping.exe
	cp -p cmds.inf $(PKGDIR)

bin/install/bin/find.exe: src/cmds/find.c
	$(CC) -o $@ $^ $(CCFLAGS) -DUSE_LOCAL_HEAP -noshare

bin/install/bin/grep.exe: src/cmds/grep.c
	$(CC) -o $@ $^ $(CCFLAGS)

bin/install/bin/ping.exe: src/cmds/ping.c
	$(CC) -o $@ $^ $(CCFLAGS)

dev: bin/install/usr/bin/cc.exe bin/install/usr/bin/as.exe
	cp -p sdk.inf $(PKGDIR)

bin/install/usr/bin/cc.exe: $(CC_SRCFILES) $(CC_HDRFILES)
	$(CC) -o $@ $(CC_SRCFILES) -I sdk/cc $(CCFLAGS) -DUSE_LOCAL_HEAP -noshare
	cp -p cc.inf $(PKGDIR)

bin/install/usr/bin/as.exe: $(AS_SRCFILES)
	$(CC) -o $@ $^ -I sdk/as $(CCFLAGS) $(AS_CFLAGS) -DUSE_LOCAL_HEAP -noshare
	cp -p as.inf $(PKGDIR)

vmdk: bin/img/ckernel.vmdk

bin/img/ckernel.vmdk: ckernel
	$(MKPKG) bin/install - $(PKGDIR)/*.inf
	mv db $(PKGDIR)
	$(MKDFS) -d bin/img/ckernel.vmdk -t vmdk -b bin/install/boot/boot -l bin/install/boot/osldr.dll -k bin/install/boot/krnl.dll -c 200M -i -f -S bin/install -T /

hdmin: bin/img/hd.vmdk

bin/img/hd.vmdk: ckernel
	$(MKDFS) -d bin/img/hd.vmdk -t vmdk -b bin/install/boot/boot -l bin/install/boot/osldr.dll -k bin/install/boot/krnl.dll -c 100M -i -f -S bin/install -F hdmin.lst

iso: bin/img/ckernel.iso

bin/img/ckernel.iso: ckernel
	$(MKDFS) -d bin/install/BOOTIMG.BIN -b bin/install/boot/cdemboot -l bin/install/boot/osldr.dll -k bin/install/boot/krnl.dll -c 512 -C 1440 -I 8192 -i -f -K rootdev=cd0,rootfs=cdfs
	genisoimage -J -f -c BOOTCAT.BIN -b BOOTIMG.BIN -o bin/img/ckernel.iso bin/install
	rm bin/install/BOOTIMG.BIN

clean:
	rm -rf bin

pkgs: *.inf $(MKPKG)
	$(MKPKG) bin/install $(PKGREPO) *.inf
