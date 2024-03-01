all: cc.exe

CC_SRCFILES=asm386.c asm.c cc.c codegen386.c codegen.c compiler.c elf.c pe.c preproc.c symbol.c type.c util.c
CC_HDRFILES=cc.h config.h elf.h opcodes.h tokens.h

cc.exe: $(CC_SRCFILES) $(CC_HDRFILES)
    $(CC) -o cc.exe $(CC_SRCFILES) -DUSE_LOCAL_HEAP -noshare

install: cc.exe
    cp cc.exe $(ROOT)/usr/bin/cc.exe

cross: install

clean:
    rm cc.exe
