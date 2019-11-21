#include "pic.h"
#include "asm.h"

void pic_init(void)
{
    asm_out8(PIC_MASTER_PORT1, 0x11);
    asm_out8(PIC_MASTER_PORT2, PIC_INTERRUPT_NUMBER);
    asm_out8(PIC_MASTER_PORT2, 0x04);
    asm_out8(PIC_MASTER_PORT2, 0x01); // manual EOI

    asm_out8(PIC_SLAVE_PORT1, 0x11);
    asm_out8(PIC_SLAVE_PORT2, PIC_INTERRUPT_NUMBER + 8);
    asm_out8(PIC_SLAVE_PORT2, 0x02);
    asm_out8(PIC_SLAVE_PORT2, 0x01); // manual EOI

    pic_set_mask(~PIC_MASKBIT_SLAVE);
}

void pic_set_mask(uint16_t mask)
{
    asm_out8(PIC_MASTER_PORT2, (uint8_t)mask);
    asm_out8(PIC_SLAVE_PORT2, (uint8_t)(mask >> 8));
}

void pic_send_eoi(uint8_t irq)
{
    if (irq < 8)
    {
        asm_out8(PIC_MASTER_PORT1, 0x60 | irq);
    }
    else
    {
        asm_out8(PIC_MASTER_PORT1, 0x60 | PIC_IRQ_SLAVE);
        asm_out8(PIC_SLAVE_PORT1, 0x60 | (irq - 8));
    }
}
