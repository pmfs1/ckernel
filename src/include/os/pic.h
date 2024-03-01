//
// Programmable Interrupt Controller (PIC i8259)
//
#ifndef PIC_H
#define PIC_H

//
// I/O port addresses
//

#define PIC_MSTR_CTRL           0x20
#define PIC_MSTR_MASK           0x21

#define PIC_SLV_CTRL            0xA0
#define PIC_SLV_MASK            0xA1

//
// Master commands
//

#define PIC_MSTR_ICW1           0x11
#define PIC_MSTR_ICW2           0x20
#define PIC_MSTR_ICW3           0x04
#define PIC_MSTR_ICW4           0x01

//
// Slave commands
//

#define PIC_SLV_ICW1            0x11
#define PIC_SLV_ICW2            0x28
#define PIC_SLV_ICW3            0x02
#define PIC_SLV_ICW4            0x01

//
// End of interrupt commands
//

#define PIC_EOI_BASE            0x60

#define PIC_EOI_CAS             0x62
#define PIC_EOI_FD              0x66

void init_pic();

krnlapi void enable_irq(unsigned int irq);

krnlapi void disable_irq(unsigned int irq);

krnlapi void eoi(unsigned int irq);

#endif
