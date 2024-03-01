;-----------------------------------------------------------------------------
; cos.asm - floating point cosine
;-----------------------------------------------------------------------------

                SECTION .text

                global  cos
                global  _cos
                
cos:
_cos:
                push    ebp
                mov     ebp,esp                 ; Point to the stack frame
                fld     qword [ebp+8]           ; Load real from stack
                fcos                            ; Take the cosine
                pop     ebp
                ret
