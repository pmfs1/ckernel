;-----------------------------------------------------------------------------
; fpreset.asm - floating point unit reset
;-----------------------------------------------------------------------------

                SECTION .text

                global  _fpreset
                global  __fpreset
                
_fpreset:
__fpreset:
                finit                           ; Initialize the FPU
                ret
