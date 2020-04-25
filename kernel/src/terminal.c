#include "terminal.h"
#include "keycode.h"
#include "string.h"
#include "asm.h"
#include <stdarg.h>

static uint16_t* const buffer = (uint16_t*)0xffff80001feb8000;
static struct term_pos scrcur = { 0, 0 };

static struct term_pos inpcur = { 0, 24 };
static uint8_t input_beg = 0;
static char last_input[81];

enum term_mode { NORMAL, INPUT };
static int mode = NORMAL;

static struct term_pos term_write_char_at(uint8_t color, uint8_t col, uint8_t row, char ch);
static void scroll();
static void update_cursor();

void term_init(void)
{
    for (unsigned index = 0; index < 80 * 24; ++index)
    {
        buffer[index] = TERM_COLOR_DEFAULT << 8;
    }

    term_enable_cursor(true);
    term_mode_normal();
}

void term_enable_cursor(bool enable)
{
    if (enable)
    {
        asm_out8(0x3d4, 0x0a);
        asm_out8(0x3d5, (asm_in8(0x3d5) & 0xc0) | 13);
        asm_out8(0x3d4, 0x0b);
        asm_out8(0x3d5, (asm_in8(0x3d5) & 0xc0) | 15);
    }
    else
    {
        asm_out8(0x3d4, 0x0a);
        asm_out8(0x3d5, 0x20);
    }
}

void term_set_status_color(uint8_t color)
{
    for (unsigned index = 80 * 24; index < 80 * 25; ++index)
    {
        buffer[index] = color << 8;
    }
}

static struct term_pos term_write_char_at(uint8_t color, uint8_t col, uint8_t row, char ch)
{
    if (ch == '\n')
    {
        if (++row >= 24)
        {
            --row;
            scroll();
        }
        return (struct term_pos){ .col = 0, .row = row };
    }
    else if (ch >= 0x20)
    {
        buffer[80 * row + col] = (color << 8) | ch;

        if (++col >= 80)
        {
            if (++row >= 24)
            {
                --row;
                scroll();
            }
            col = 0;
        }
        return (struct term_pos){ .col = col, .row = row };
    }
    else
    {
        return (struct term_pos){ .col = col, .row = row };
    }

}

struct term_pos term_write_string_at(uint8_t color, uint8_t col, uint8_t row, const char* line)
{
    struct term_pos pos = { .col = col, .row = row };
    for (; *line != 0; ++line)
    {
        pos = term_write_char_at(color, pos.col, pos.row, *line);
    }
    return pos;
}

void term_write_string(uint8_t color, const char* line)
{
    scrcur = term_write_string_at(color, scrcur.col, scrcur.row, line);
    update_cursor();
}

struct term_pos term_write_format_at(uint8_t color, uint8_t col, uint8_t row, const char* format, ...)
{
    struct term_pos pos;
    char buf[1024];
    va_list va;
    va_start(va, format);
    vsnprintf(buf, sizeof(buf), format, va);
    pos = term_write_string_at(color, col, row, buf);
    va_end(va);
    return pos;
}

void term_write_format(uint8_t color, const char* format, ...)
{
    char buf[1024];
    va_list va;
    va_start(va, format);
    vsnprintf(buf, sizeof(buf), format, va);
    term_write_string(color, buf);
    va_end(va);
}

void term_write_status(const char* status)
{
    unsigned idx = 0;
    for (idx = 0; idx < 80 && status[idx] != 0; ++idx)
    {
        buffer[80 * 24 + idx] &= 0xff00;
        buffer[80 * 24 + idx] |= status[idx];
    }
    for (; idx < 80; ++idx)
    {
        buffer[80 * 24 + idx] &= 0xff00;
    }
}

void term_write_stat_f(const char* format, ...)
{
    char status[80] = { 0 };
    va_list va;
    va_start(va, format);
    vsnprintf(status, sizeof(status), format, va);
    term_write_status(status);
    va_end(va);
}

void term_panic(const char* message)
{
    term_write_string(TERM_COLOR_PANIC, message);
    while (1)
    {
        __asm__ __volatile__ ( "cli \r\n hlt" );
    }
}

void term_mode_normal(void)
{
    mode = NORMAL;
    term_set_status_color(TERM_COLOR_STATUS);
    term_write_status("-- NORMAL --");
    update_cursor();
}

void term_mode_input(const char* prompt)
{
    int len = strlen(prompt);

    mode = INPUT;
    term_set_status_color(TERM_COLOR_INPUT);
    term_write_stat_f("%78s", prompt);
    inpcur.col = input_beg = min(len, 78);
    update_cursor();
}

void term_on_input(uint8_t cascii)
{
    if (mode != NORMAL && cascii == CASCII_ESC)
    {
        term_mode_normal();
        return;
    }
    else if (mode == NORMAL && cascii == ':')
    {
        term_mode_input(":");
        return;
    }
    else if (mode != INPUT)
    {
        return;
    }

    if (cascii == '\n')
    {
        for (int idx = 0; idx < 80 - input_beg; ++idx)
        {
            last_input[idx] = buffer[80 * 24 + idx + input_beg] & 0xff;
            if (last_input[idx] == 0)
                break;
        }
        last_input[sizeof(last_input) - 1] = 0;
        term_mode_normal();
    }
    else if (cascii == '\b')
    {
        if (inpcur.col > input_beg)
        {
            buffer[80 * 24 + --inpcur.col] &= 0xff00;
        }
    }
    else if (0x20 <= cascii && cascii <= 0x7f)
    {
        if (inpcur.col < 79)
        {
            buffer[80 * 24 + inpcur.col] &= 0xff00;
            buffer[80 * 24 + inpcur.col++] |= cascii;
        }
    }
    update_cursor();
}

static void scroll()
{
    memmove(buffer, buffer + 80, 2 * 80 * 23);
    memset(buffer + 80 * 23, 0, 2 * 80);
    if (scrcur.row > 0)
    {
        --scrcur.row;
    }
    update_cursor();
}

static void update_cursor()
{
    struct term_pos cur = (mode == NORMAL) ? scrcur : inpcur;
    uint16_t pos = cur.row * 80 + cur.col;
    asm_out8(0x3d4, 0x0f);
    asm_out8(0x3d4 + 1, (uint8_t)pos);
    asm_out8(0x3d4, 0x0e);
    asm_out8(0x3d4 + 1, (uint8_t)(pos >> 8));
}
