#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "terminal.h"
#include "intr_queue.h"
#include "asm.h"

static void intr_loop(void);

void kmain(void)
{
    term_init();

    gdt_table_init();
    idt_table_init();

    pic_init();
    intr_queue_init();

    keyboard_init();

    pic_set_mask(~(PIC_MASKBIT_SLAVE | PIC_MASKBIT_KEYBOARD));
    asm_sti();

    intr_loop();
}

static void intr_loop(void)
{
    uint32_t value;
    while (1)
    {
        while (intr_queue_pop_syslock(&value))
        {
            uint32_t data = value & INTR_QUEUE_MASK_DATA;
            switch (value & INTR_QUEUE_MASK_FLAG)
            {
                case INTR_QUEUE_FLAG_KEYBOARD:
                    intr_queue_keyboard(data);
                    break;
            }
        }

        asm_hlt();
    }
}
