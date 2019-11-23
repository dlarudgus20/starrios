#include "keyboard.h"
#include "pic.h"
#include "gdt.h"
#include "idt.h"
#include "intr_queue.h"
#include "syslock.h"
#include "terminal.h"
#include "asm.h"

enum
{
    ACK_TRY_COUNT = 100
};

struct keyboard g_keyboard;

bool keyboard_init(void)
{
    asm_out8(KEYBOARD_PORT_CONTROL, KEYBOARD_CTRL_ACTIVATE_KEYBOARD);
    keyboard_waitforinput();
    asm_out8(KEYBOARD_PORT_INPUT, KEYBOARD_COMMAND_ACTIVATE);
    if (!keyboard_waitforACK(false))
        return false;

    keyboard_update_led(false);

    idt_init(g_idt_table + PIC_INTERRUPT_KEYBOARD, KERNEL_CODE_SEG, int_keyboard);
    return true;
}

bool keyboard_update_led(bool polling)
{
    bool ret = true;
    syslock_t syslock = syslock_acquire();

    keyboard_waitforinput();

    asm_out8(KEYBOARD_PORT_INPUT, KEYBOARD_COMMAND_LED);
    keyboard_waitforinput();
    if (keyboard_waitforACK(polling))
    {
        uint8_t code = (g_keyboard.capslock << 2) | (g_keyboard.numlock << 1) | g_keyboard.scrolllock;
        asm_out8(KEYBOARD_PORT_INPUT, code);
        keyboard_waitforinput();
        ret = keyboard_waitforACK(polling);
    }

    syslock_release(syslock);
    return ret;
}

bool keyboard_waitforACK(bool polling)
{
    for (int i = 0; i < ACK_TRY_COUNT; ++i)
    {
        for (int j = 0; j < 0xffff; ++j)
        {
            if (keyboard_is_outputbuf_full())
                break;
        }

        uint8_t data = asm_in8(KEYBOARD_PORT_OUTPUT);
        if (data == KEYBOARD_ACK)
        {
            return true;
        }
        else if (polling)
        {
            intr_queue_push(data | INTR_QUEUE_FLAG_KEYBOARD);
        }
    }
    return false;
}

void keyboard_waitforinput(void)
{
    for (int j = 0; j < 0xffff; ++j)
    {
        if (keyboard_is_inputbuf_full())
            break;
    }
}

noreturn void keyboard_resetPC(void)
{
    keyboard_waitforinput();
    asm_out8(KEYBOARD_PORT_CONTROL, KEYBOARD_CTRL_WRITE_OUTP);
    asm_out8(KEYBOARD_PORT_INPUT, 0 /* reset */);
    while (1) { } // noreturn
}

bool keyboard_is_inputbuf_full(void)
{
    return asm_in8(KEYBOARD_PORT_STATUS) & KEYBOARD_STATUS_INPB;
}

bool keyboard_is_outputbuf_full(void)
{
    return asm_in8(KEYBOARD_PORT_STATUS) & KEYBOARD_STATUS_OUTB;
}

void intc_keyboard(struct interrupt_context* rsp)
{
    pic_send_eoi(PIC_IRQ_KEYBOARD);
    uint8_t code = asm_in8(KEYBOARD_PORT_OUTPUT);

    if (code != KEYBOARD_ACK)
    {
        intr_queue_push(code | INTR_QUEUE_FLAG_KEYBOARD);
    }
}

void intr_queue_keyboard(uint32_t data)
{
    static char tbl[] = "0123456789abcdef";
    static int x = 0;
    term_put_char_at(tbl[(data >> 4) & 0xf], x++, 1);
    term_put_char_at(tbl[data & 0xf], x++, 1);

    uint16_t keycode;
    uint8_t scan = data & 0xff;
    if (assemble_keycode_from_scan_sequence(&g_keyboard.kca_state, scan, &keycode))
    {
        uint8_t cascii = keycode_to_cascii(keycode);
        g_key_table[keycode & 0xff].pushed = keycode_is_pushed(keycode);

        switch (cascii)
        {
            case CASCII_LSHIFT:
            case CASCII_RSHIFT:
                g_keyboard.shift = (g_key_table[SCAN_CODE_LSHIFT].pushed
                    || g_key_table[SCAN_CODE_RSHIFT].pushed);
                break;
            case CASCII_NUMLOCK:
                if (!keycode_is_pushed(keycode))
                {
                    g_keyboard.numlock = !g_keyboard.numlock;
                    keyboard_update_led(true);
                }
                break;
            case CASCII_CAPSLOCK:
                if (!keycode_is_pushed(keycode))
                {
                    g_keyboard.capslock = !g_keyboard.capslock;
                    keyboard_update_led(true);
                }
                break;
            case CASCII_SCROLLLOCK:
                if (!keycode_is_pushed(keycode))
                {
                    g_keyboard.scrolllock = !g_keyboard.scrolllock;
                    keyboard_update_led(true);
                }
                break;
        }

        term_put_char_at(tbl[(cascii >> 4) & 0xf], 0, 0);
        term_put_char_at(tbl[cascii & 0xf], 1, 0);
        term_put_char_at(cascii, 3, 0);

        if (cascii != 0 && keycode_is_pushed(keycode))
        {
            //term_on_input(cascii);
        }
    }
}
