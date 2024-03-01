;
; Boot sector
;

OSLDRSEG    equ 0x9000

OSLDRSTART  equ 8
OSLDRSIZE   equ 32

        ORG     0x7c00
        BITS    16
        SECTION .text

;
; Entry point for initial bootstap code
;

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

        ; Save boot drive
        mov     [bootdrv], dl

        ; Display boot message
        mov     si, bootmsg
        call    print

readldr:
        ; Get boot drive geometry
        mov     dl, [bootdrv]
        mov     ah, 8
        xor     di, di
        mov     es, di
        int     0x13

        and     cl, 0x3F
        mov     byte [sectors], cl
        inc     dh
        mov     byte [heads], dh

        ; Load os loader from boot drive

loadnext:
        xor     eax, eax
        mov     ax, [sectno]        ; eax = os loader sector number

        mov     bx, ax
        shl     bx, 5               ; 512/16 segments per sector                    
        add     bx, OSLDRSEG
        mov     es, bx              ; es = segment for next os loader sector

        mov     di, [ldrsize]
        sub     di, ax              ; di = number of os loader sectors left to read

        mov     cx, [sectno]        ; make sure we do not cross a 64KB boundary
        and     cx, 0x7F
        neg     cx
        add     cx, 0x80
        cmp     cx, di
        jg      loadnext1
        mov     di, cx
loadnext1:
        mov     ebx, [ldrstrt]      ; eax = LBA of next os loader sector
        add     eax, ebx
        call    readsectors

        mov     ax, [sectno]        ; update next os loader sector to read
        add     ax, di
        mov     [sectno], ax

        cmp     ax, [ldrsize]
        jnz     loadnext

        ; Call real mode entry point in os loader
        mov     ax, OSLDRSEG
        mov     ds, ax
        add     ax, [0x16]          ; cs
        push    ax
        push    word [0x14]         ; ip

        mov     dl, [cs:bootdrv]    ; boot drive
        xor     ebx, ebx            ; RAM disk image
        retf
        
;
; Read sectors from boot drive
; input:
;   eax = LBA
;   di = maximum sector count
;   es = segment for buffer
; output:
;   di = sectors read
;

readsectors:
        ; Convert LBA to CHS
        cdq                         ; edx = 0
        movzx   ebx, word [sectors]
        div     ebx                 ; eax = track, edx = sector - 1
        mov     cx, dx              ; cl = sector - 1, ch = 0
        inc     cx                  ; cl = sector number
        xor     dx, dx
        mov     bl, [heads]
        div     ebx

        mov     dh, dl              ; head
        mov     dl, [bootdrv]       ; boot drive
        xchg    ch, al              ; ch = low 8 bits of cylinder number, al = 0
        shr     ax, 2               ; al[6:7] = high two bits of cylinder, ah = 0
        or      cl, al              ; cx = cylinder and sector

        ; Determine number of sectors to read
        mov     al, cl
        mov     ah, 0
        add     ax, di              ; ax = last sector to xfer
        cmp     ax, [sectors]
        jbe     readall

        mov     ax, [sectors]       ; read to end of track
        inc     ax
        sub     al, cl
        push    ax
        jmp     read

readall:
        mov     ax, di              ; we can read all sectors
        push    ax
        jmp     read

readagain:
        pusha
        mov     al, '#'             ; print # to mark errror
        call    printchar
        mov     ax, 0               ; reset disk system
        mov     dx, 0
        int     0x13
        popa

read:
        mov     ah, 2               ; read sector using bios
        xor     bx, bx
        int     0x13
        jc      readagain

        pop     di

        ret

;               
; Print string to console
;   si = ptr to first character of a null terminated string
;

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

;
; Print a single character to the console
;   al = character to be printed
;

printchar:
        mov     ah, 0x0e
        int     0x10
        ret

;
; Variables
;

sectno  dw      0
sectors dw      0
heads   dw      0
bootdrv db      0

;
; Message strings
;

bootmsg:
        db      'Booting... bootstrap', 0

;
; Boot signature
;

        times   510-($-$$) db 0
        dw      0xAA55
