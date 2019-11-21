#include "terminal.h"
#include "string.h"
#include <stdarg.h>

static struct terminal s_term;

void term_init(void)
{
    s_term.row_size = 23;
    s_term.col_size = 80;
    s_term.col_bits = 7;

    s_term.cur_row = 0;
    s_term.cur_col = 0;
    s_term.scr_row = 0;

    s_term.status = 0;
    s_term.color = TERM_COLOR_DEFAULT;

    array8_init(&s_term.linebuf, s_term.linebuf_raw,
        s_term.linebuf_raw + sizeof(s_term.linebuf_raw));
    circular_queue8_init(&s_term.hitkeybuf, s_term.hitkeybuf_raw, sizeof(s_term.hitkeybuf));

    memset(s_term.scrbuf, 0, sizeof(s_term.scrbuf));
}

void term_set_color(uint8_t color)
{
    s_term.color = color;
}

void term_set_status(enum term_status status)
{
    s_term.status = status;
}

static void scroll_impl(uint8_t count)
{
    uint8_t* video = (uint8_t*)0xffff80001feb80a0;
    unsigned entries = (s_term.row_size - count) << s_term.col_bits;
    memmove(video, video + 2 * (entries << s_term.col_bits), 2 * entries);
}

static void put_entry_impl(char c, uint8_t color, uint16_t x, uint16_t y)
{
    uint16_t* video = (uint16_t*)0xffff80001feb80a0;
    video[y * 80 + x] = (uint8_t)c | (color << 8);
}

void term_scroll(uint8_t count)
{
    uint8_t rowlimit = TERM_SCRBUF_SIZE >> s_term.col_bits;
    uint8_t old_scr = s_term.scr_row;

    s_term.scr_row += count;
    if (s_term.scr_row >= rowlimit - s_term.row_size)
    {
        uint8_t gap_row = s_term.scr_row - rowlimit + s_term.row_size + 1;
        uint8_t gap_ch = gap_row << s_term.col_bits;
        memmove(s_term.scrbuf, s_term.scrbuf + gap_ch, sizeof(uint16_t) * (TERM_SCRBUF_SIZE - gap_ch));
        memset(s_term.scrbuf + TERM_SCRBUF_SIZE - gap_ch, 0, sizeof(uint16_t) * gap_ch);
        s_term.scr_row -= gap_row;
        old_scr -= gap_row;
    }

    scroll_impl(count);

    for (uint8_t y = count; y < s_term.row_size; ++y)
    {
        for (uint8_t x = 0; x < s_term.col_size; ++x)
        {
            uint16_t value = s_term.scrbuf[(y << s_term.col_bits) + x];
            put_entry_impl((char)(value & 0xff), value >> 8, x, y);
        }
    }
}

void term_put_entry_at(char c, uint8_t color, uint16_t x, uint16_t y)
{
    s_term.scrbuf[((s_term.scr_row + y) << s_term.col_bits) + x] = (color << 8) | (uint8_t)c;
    put_entry_impl(c, color, x, y);
}

void term_on_input(uint8_t cascii)
{
    circular_queue8_put(&s_term.hitkeybuf, cascii);
    if (s_term.status != TERM_STATUS_STRING_INPUTING)
        return;
}

void term_put_char_at(char c, uint16_t x, uint16_t y)
{
    term_put_entry_at(c, s_term.color, x, y);
}

void term_put_char(char c)
{
    if (c == '\n')
    {
        term_newline();
    }
    else if (c == '\b')
    {
        term_backspace();
    }
    else if (c != '\0')
    {
        // todo
        //term_put_char_at(c, s_term.cur_col, s_term.cur_row);
    }
}

void term_newline(void)
{

}

void term_delete(void)
{

}

void term_backspace(void)
{

}

void term_cls(void)
{

}

void term_write_string_at(uint16_t x, uint16_t y, uint8_t color, const char* str)
{

}

void term_write_format_at(uint16_t x, uint16_t y, uint8_t color, const char* format, ...)
{

}

void term_write_status_string(const char* str)
{

}

void term_write_status_format(const char* format, ...)
{

}

void term_clear_status(void)
{

}

void term_panic(const char* str)
{
    __asm__ __volatile__ ( "cli" );

    uint8_t* video = (uint8_t*)0xffff80001feb8000;
    for (const char* p = str; *p != '\0'; ++p)
    {
        *video++ = *p;
        *video++ = TERM_COLOR_PANIC;
    }

    while (1) __asm__ __volatile__ ( "hlt" );
}

void term_set_cursor_width(uint8_t begin_height, uint8_t end_height)
{

}

void term_enable_cursor(bool enabled)
{

}

uint16_t term_wherex(void)
{
    return 0xdead;
}

uint16_t term_wherey(void)
{
    return 0xbeef;
}

void term_gotoxy(uint16_t x, uint16_t y)
{

}

void term_print_string(const char* str)
{

}

void term_print_format(const char* format, ...)
{

}

uint8_t term_get_cascii(void)
{
    return 0xc8;
}

size_t term_getline(char* buf, size_t size)
{
    return 0xf00ba2;
}

char term_getchar(void)
{
    return '\v';
}

void term_clear_line_buffer()
{

}

void term_clear_input_buffer(void)
{

}

void term_clear_hitkey_buffer(void)
{

}

void term_clear_all_buffers(void)
{

}

bool term_hitkey_exists(void)
{
    return false;
}
