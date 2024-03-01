;
; PXE Network Boot sector
;

OSLDRSEG    equ 0x9000
OSLDRBASE   equ (OSLDRSEG * 16)

OSLDRSTART  equ 8
OSLDRSIZE   equ 32

        ORG     0x7c00
        BITS    16
        SECTION .text

        ; Entry point for initial bootstap code
boot:
        jmp     short start
        nop
        nop
        
        db      'CKERNEL   '

ldrsize  dw     OSLDRSIZE
ldrstrt  dd     OSLDRSTART

start:
        ; Setup initial environment
        jmp     0:start1
start1:
        mov     ax, cs
        mov     ds, ax
        
        mov     ax, 0x9000
        mov     ss, ax
        mov     sp, 0xF800

        ; Display boot message
        mov     si, bootmsg
        call    print
        
        ; Copy os loader from ram boot image 
        push    ds

        mov     cx, [ldrsize]       ; cx = number of bytes to copy
        shl     cx, 9               ; convert from sectors to bytes

        mov     eax, [ldrstrt]      ; ds = loader start segment
        shl     eax, 5              ; convert from sectors to segments
        add     eax, 0x7c0
        mov     ds, ax

        mov     ax, OSLDRSEG        ; es = loader segment
        mov     es, ax

        xor     di, di
        xor     si, si

        cld                         ; copy os loader from boot image
        rep movsb
        pop     ds

        ; Call real mode entry point in os loader
        mov     ax, OSLDRSEG
        mov     ds, ax
        add     ax, [0x16]          ; cs
        push    ax
        push    word [0x14]         ; ip

        mov     dl, 0xFE            ; boot drive (0xFE for PXE) 
        mov     ebx, 0x7C00         ; RAM disk image
        retf

        ; Print string to console
        ; si = ptr to first character of a null terminated string
print:
        push    ax
        cld
nextchar:
        mov     al, [si]
        cmp     al, 0
        je      printdone
        call    printchar
        inc     si
        jmp     nextchar
printdone:
        pop     ax
        ret

        ; Print a single character to the console
        ; al = character to be printed
printchar:
        mov     ah, 0x0e
        int     0x10
        ret

        ; Message strings
bootmsg:
        db      'Booting system from network... ', 0

        ; Boot signature
        times   510-($-$$) db 0
        dw      0xAA55
