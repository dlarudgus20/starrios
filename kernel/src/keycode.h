#pragma once

#include <stdint.h>
#include <stdbool.h>

// starrios는 3가지의 keycode system을 가집니다.
// scan code는 키보드 하드웨어가 사용하는 값이며, 1byte ~ 3byte의 가변 크기입니다.
// keycode는 scan code를 1byte로 합치고, push/unpush 여부를 9bit에 기록하는 코드입니다.
// cascii는 ascii 호환 코드로, 0x81 이후의 값을 ascii 범위 밖 키를 표현하는 데 사용합니다.

// cascii code
enum
{
    // ascii keys
    CASCII_NONE             = 0x00,
    CASCII_ENTER            = '\n',
    CASCII_TAB              = '\t',
    CASCII_ESC              = 0x1B,
    CASCII_BACKSPACE        = 0x08,

    // non-ascii keys
    CASCII_LCTRL            = 0x81,
    CASCII_LSHIFT           = 0x82,
    CASCII_RSHIFT           = 0x83,
    CASCII_PRINTSCREEN      = 0x84,
    CASCII_LALT             = 0x85,
    CASCII_CAPSLOCK         = 0x86,
    CASCII_F1               = 0x87,
    CASCII_F2               = 0x88,
    CASCII_F3               = 0x89,
    CASCII_F4               = 0x8A,
    CASCII_F5               = 0x8B,
    CASCII_F6               = 0x8C,
    CASCII_F7               = 0x8D,
    CASCII_F8               = 0x8E,
    CASCII_F9               = 0x8F,
    CASCII_F10              = 0x90,
    CASCII_NUMLOCK          = 0x91,
    CASCII_SCROLLLOCK       = 0x92,
    CASCII_NUM_HOME         = 0x93,
    CASCII_NUM_UP           = 0x94,
    CASCII_NUM_PAGEUP       = 0x95,
    CASCII_NUM_LEFT         = 0x96,
    CASCII_NUM_CENTER       = 0x97,
    CASCII_NUM_RIGHT        = 0x98,
    CASCII_NUM_END          = 0x99,
    CASCII_NUM_DOWN         = 0x9A,
    CASCII_NUM_PAGEDOWN     = 0x9B,
    CASCII_NUM_INS          = 0x9C,
    CASCII_NUM_DEL          = 0x9D,
    CASCII_F11              = 0x9E,
    CASCII_F12              = 0x9F,

    // extended scan code keys
    CASCII_NUM_ENTER        = 0xA0,
    CASCII_RCTRL            = 0xA1,
    CASCII_NUM_SLASH        = 0xA2,
    CASCII_RALT             = 0xA3,
    CASCII_HOME             = 0xA4,
    CASCII_UP               = 0xA5,
    CASCII_PAGEUP           = 0xA6,
    CASCII_LEFT             = 0xA7,
    CASCII_RIGHT            = 0xA8,
    CASCII_END              = 0xA9,
    CASCII_DOWN             = 0xAA,
    CASCII_PAGEDOWN         = 0xAB,
    CASCII_INSERT           = 0xAC,
    CASCII_DELETE           = 0xAD,

    CASCII_PAUSE            = 0xAE,
};

// keycode
enum
{
    KEYCODE_NUM_MIN         = 0x47,
    KEYCODE_NUM_MAX         = 0x53,

    KEYCODE_PAUSE           = 0x91,

    KEYCODE_FLAG_UNPUSH     = 0x100,
};

// scan code
enum
{
    SCAN_CODE_RSHIFT        = 0x36,
    SCAN_CODE_LSHIFT        = 0x2A,
};

struct key_data
{
    uint8_t normal;     ///< 다른 키가 눌리지 않았을 때의 cascii code
    uint8_t combined;   ///< Shift, Caps Lock, Num Lock과 조합됬을 때의 cascii code
    bool pushed;        ///< 키가 눌려있는지의 여부
};

extern struct key_data g_key_table[];

struct keycode_assembler
{
    bool extended;
    bool pause;
    unsigned skip_for_pause;
};

static inline bool keycode_is_pushed(uint16_t keycode)
{
    return !(keycode & KEYCODE_FLAG_UNPUSH);
}

void keycode_assembler_init(struct keycode_assembler* assembler);
bool assemble_keycode_from_scan_sequence(struct keycode_assembler* assembler,
    uint8_t scan, uint16_t* keycode);

uint16_t keycode_from_scancode(uint8_t scan, bool extended, bool pause);
uint8_t keycode_to_cascii(uint16_t keycode);
