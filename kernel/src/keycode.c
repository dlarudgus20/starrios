#include "keycode.h"
#include "keyboard.h"
#include "string.h"

void keycode_assembler_init(struct keycode_assembler* assembler)
{
    assembler->extended = false;
    assembler->pause = false;
    assembler->skip_for_pause = 0;
}

bool assemble_keycode_from_scan_sequence(struct keycode_assembler* assembler,
    uint8_t scan, uint16_t* keycode)
{
    if (assembler->pause)
    {
        if (--assembler->skip_for_pause != 0)
        {
            return false;
        }
        else
        {
            assembler->pause = false;
            *keycode = KEYCODE_PAUSE;
            return true;
        }
    }
    else if (assembler->extended)
    {
        assembler->extended = false;
        *keycode = keycode_from_scancode(scan, true, false);
        return true;
    }
    else
    {
        if (scan == 0xe1)
        {
            assembler->pause = true;
            assembler->skip_for_pause = 2;
            return false;
        }
        else if (scan == 0xe0)
        {
            assembler->extended = true;
            return false;
        }
        else
        {
            *keycode = keycode_from_scancode(scan, false, false);
            return true;
        }
    }

}

uint16_t keycode_from_scancode(uint8_t scan, bool extended, bool pause)
{
    bool pushed = true;
    uint16_t keycode;

    if (scan & 0x80)
    {
        pushed = false;
        scan &= 0x7f;
    }

    if (pause)
    {
        keycode = KEYCODE_PAUSE;
    }
    else if (extended)
    {
        keycode = scan - 0x1c + 0x59;
    }
    else
    {
        keycode = scan;
    }

    return pushed ? keycode : (keycode | KEYCODE_FLAG_UNPUSH);
}

uint8_t keycode_to_cascii(uint16_t keycode)
{
    struct key_data key;

    keycode &= 0xff;
    key = g_key_table[keycode];
    if (keycode >= KEYCODE_NUM_MIN && KEYCODE_NUM_MAX)
    {
        return g_keyboard.numlock ? key.combined : key.normal;
    }
    else if (isalpha(key.normal))
    {
        return (g_keyboard.capslock != g_keyboard.shift) ? key.combined : key.normal;
    }
    else
    {
        return g_keyboard.shift ? key.combined : key.normal;
    }
}
