//
// Programmable Interrupt Controller (PIC i8259)
//
#include <os/krnl.h>

// All IRQs disabled initially except cascade

unsigned int irq_mask = 0xFFFB;

//
// Set interrupt mask
//

static void set_intr_mask(unsigned long mask) {
    outp(PIC_MSTR_MASK, (unsigned char) mask);
    outp(PIC_SLV_MASK, (unsigned char) (mask >> 8));
}

//
// Initialize the 8259 Programmable Interrupt Controller
//

void init_pic() {
    outp(PIC_MSTR_CTRL, PIC_MSTR_ICW1);
    outp(PIC_SLV_CTRL, PIC_SLV_ICW1);
    outp(PIC_MSTR_MASK, PIC_MSTR_ICW2);
    outp(PIC_SLV_MASK, PIC_SLV_ICW2);
    outp(PIC_MSTR_MASK, PIC_MSTR_ICW3);
    outp(PIC_SLV_MASK, PIC_SLV_ICW3);
    outp(PIC_MSTR_MASK, PIC_MSTR_ICW4);
    outp(PIC_SLV_MASK, PIC_SLV_ICW4);

    set_intr_mask(irq_mask);
}

//
// Enable IRQ
//

void enable_irq(unsigned int irq) {
    irq_mask &= ~(1 << irq);
    if (irq >= 8) irq_mask &= ~(1 << 2);
    set_intr_mask(irq_mask);
}

//
// Disable IRQ
//

void disable_irq(unsigned int irq) {
    irq_mask |= (1 << irq);
    if ((irq_mask & 0xFF00) == 0xFF00) irq_mask |= (1 << 2);
    set_intr_mask(irq_mask);
}

//
// Signal end of interrupt to PIC
//

void eoi(unsigned int irq) {
    if (irq < 8) {
        outp(PIC_MSTR_CTRL, irq + PIC_EOI_BASE);
    } else {
        outp(PIC_SLV_CTRL, (irq - 8) + PIC_EOI_BASE);
        outp(PIC_MSTR_CTRL, PIC_EOI_CAS);
    }
}
