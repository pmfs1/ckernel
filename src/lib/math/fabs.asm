;-----------------------------------------------------------------------------
; fabs.asm - floating point absolute value
;-----------------------------------------------------------------------------

                SECTION .text

                global  fabs
                global  _fabs
                
fabs:
_fabs:
                push    ebp
                mov     ebp,esp
                fld     qword [ebp+8]           ; Load real from stack
                fabs                            ; Take the absolute value
                pop     ebp
                ret
