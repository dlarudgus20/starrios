#include "gdt.h"
#include "idt.h"
#include "pic.h"
#include "keyboard.h"
#include "memory.h"
#include "terminal.h"
#include "coshell.h"
#include "intr_queue.h"
#include "asm.h"

static void intr_loop(void);

void kmain(void)
{
    term_init();
    coshell_init();

    gdt_table_init();
    idt_table_init();

    pic_init();
    intr_queue_init();

    keyboard_init();

    mem_init();

    pic_set_mask(~(PIC_MASKBIT_SLAVE | PIC_MASKBIT_KEYBOARD));
    asm_sti();

    term_write_string(TERM_COLOR_LOG, "boot...ok\n");

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
