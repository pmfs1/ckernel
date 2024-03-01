OUTDEFS=-DOF_ONLY -DOF_ELF32 -DOF_WIN32 -DOF_COFF -DOF_OBJ -DOF_BIN -DOF_DBG -DOF_DEFAULT=of_elf32
CFLAGS=-I. -DHAVE_SNPRINTF -DHAVE_VSNPRINTF -DUSE_LOCAL_HEAP $(OUTDEFS)

SRCFILES=as.c aslib.c ver.c raa.c saa.c rbtree.c float.c insnsa.c insnsb.c \
         directiv.c assemble.c labels.c hashtbl.c crc64.c parser.c \
         preproc.c quote.c pptok.c macros.c listing.c eval.c exprlib.c stdscan.c \
         strfunc.c tokhash.c regvals.c regflags.c ilog2.c strlcpy.c \
         out/outform.c out/outlib.c out/nulldbg.c out/nullout.c out/outbin.c \
         out/outcoff.c out/outelf.c out/outelf32.c out/outobj.c out/outdbg.c \

all: as.exe

as.exe: $(SRCFILES)
    $(CC) $(CFLAGS) -o as.exe $(SRCFILES) -noshare

install: as.exe
    cp as.exe $(ROOT)/usr/bin/as.exe

cross: install
    
clean:
    rm as.exe
