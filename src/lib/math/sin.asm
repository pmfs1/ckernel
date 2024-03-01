;-----------------------------------------------------------------------------
; sin.asm - floating point sine
;-----------------------------------------------------------------------------

                SECTION .text

                global  sin
                global  _sin

sin:
_sin:
                push    ebp                     ; Save register bp
                mov     ebp,esp                 ; Point to the stack frame
                fld     qword [ebp+8]           ; Load real from stack
                fsin                            ; Take the sine
                pop     ebp                     ; Restore register bp
                ret                             ; Return to caller
