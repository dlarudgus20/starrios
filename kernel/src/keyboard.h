#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stdnoreturn.h>
#include "keycode.h"

enum
{
    KEYBOARD_PORT_CONTROL = 0x64,
    KEYBOARD_PORT_STATUS = 0x64,
    KEYBOARD_PORT_INPUT = 0x60,
    KEYBOARD_PORT_OUTPUT = 0x60
};

enum
{
    KEYBOARD_CTRL_ACTIVATE_MOUSE = 0xa7,
    KEYBOARD_CTRL_DEACTIVATED_MOUSE = 0xa8,
    KEYBOARD_CTRL_ACTIVATE_KEYBOARD = 0xae,
    KEYBOARD_CTRL_DEACTIVATE_KEYBOARD = 0xad,
    KEYBOARD_CTRL_READ_OUTP = 0xd0,
    KEYBOARD_CTRL_WRITE_OUTP = 0xd1
};
enum
{
    KEYBOARD_STATUS_PARE = 0x80,
    KEYBOARD_STATUS_TIM = 0x40,
    KEYBOARD_STATUS_AUXB = 0x20,
    KEYBOARD_STATUS_KEYL = 0x10,
    KEYBOARD_STATUS_C_D = 0x08,
    KEYBOARD_STATUS_SYSF = 0x04,
    KEYBOARD_STATUS_INPB = 0x02,
    KEYBOARD_STATUS_OUTB = 0x01
};

enum
{
    KEYBOARD_COMMAND_LED = 0xed,
    KEYBOARD_COMMAND_ACTIVATE = 0xf4,

    KEYBOARD_ACK = 0xfa
};

struct keyboard
{
    bool numlock;
    bool capslock;
    bool scrolllock;
    bool shift;

    struct keycode_assembler kca_state;
};

extern struct keyboard g_keyboard;

bool keyboard_init(void);
bool keyboard_update_led(bool polling);
bool keyboard_waitforACK(bool polling);
void keyboard_waitforinput(void);
noreturn void keyboard_resetPC(void);

bool keyboard_is_inputbuf_full(void);
bool keyboard_is_outputbuf_full(void);

struct interrupt_context;
void int_keyboard();
void intc_keyboard(struct interrupt_context* rsp);
void intr_queue_keyboard(uint32_t data);
