//
// Floating point unit
//
#include <os/krnl.h>

struct interrupt fpuintr;
struct interrupt fpuxcpt;

void fpu_enable(struct fpu *state) {
    // Turn on access to FPU
    set_cr0(get_cr0() & ~(CR0_EM | CR0_TS));

    if (state) {
        // Restore FPU state
        __asm {
                mov eax, state
                frstor[eax]
        }
    } else {
        // Initialize FPU
        __asm {
                fnclex
                fninit
        }
    }
}

void fpu_disable(struct fpu *state) {
    // Save FPU state
    if (state) {
        __asm {
                mov eax, state
                fnsave[eax]
        }
    }

    // Disable acces to FPU
    set_cr0(get_cr0() | CR0_EM);
}

int fpu_trap_handler(struct context *ctxt, void *arg) {
    struct thread *t = self();

    if (t->flags & THREAD_FPU_USED) {
        fpu_enable(&t->fpustate);
        t->flags |= THREAD_FPU_ENABLED;
    } else {
        fpu_enable(NULL);
        t->flags |= THREAD_FPU_ENABLED | THREAD_FPU_USED;
    }

    return 0;
}

int fpu_npx_handler(struct context *ctxt, void *arg) {
    panic("floating point processor fault");
    return 0;
}

void init_fpu() {
    register_interrupt(&fpuintr, INTR_FPU, fpu_trap_handler, NULL);
    register_interrupt(&fpuxcpt, INTR_NPX, fpu_npx_handler, NULL);
    set_cr0(get_cr0() | CR0_EM | CR0_NE);
}
