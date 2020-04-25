#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "array.h"
#include "circular_queue.h"

#define term_make_color(fg, bg) (((bg) << 4) | (fg))

enum
{
    TERM_SCRBUF_SIZE = 0x4000,
    TERM_LINBUF_SIZE = 0x400,
};

enum term_color
{
    TERM_BLACK = 0,
    TERM_BLUE = 1,
    TERM_GREEN = 2,
    TERM_CYAN = 3,
    TERM_RED = 4,
    TERM_MAGENTA = 5,
    TERM_BROWN = 6,
    TERM_LIGHT_GREY = 7,
    TERM_DARK_GREY = 8,
    TERM_LIGHT_BLUE = 9,
    TERM_LIGHT_GREEN = 10,
    TERM_LIGHT_CYAN = 11,
    TERM_LIGHT_RED = 12,
    TERM_LIGHT_MAGENTA = 13,
    TERM_LIGHT_BROWN = 14,
    TERM_WHITE= 15,

    TERM_COLOR_DEFAULT = term_make_color(TERM_LIGHT_GREY, TERM_BLACK),
    TERM_COLOR_LOG = term_make_color(TERM_LIGHT_GREEN, TERM_BLACK),
    TERM_COLOR_PANIC = term_make_color(TERM_RED, TERM_WHITE),
};

enum term_status
{
    TERM_STATUS_STRING_INPUTING,
    TERM_STATUS_WAITING,
};

struct terminal
{
    uint16_t row_size, col_size;
    uint8_t col_bits;

    uint16_t cur_row, cur_col;
    uint16_t scr_row;

    uint8_t color;
    uint8_t status;

    struct array8 linebuf;
    struct circular_queue8 hitkeybuf;

    uint8_t linebuf_raw[TERM_LINBUF_SIZE];
    uint8_t hitkeybuf_raw[TERM_LINBUF_SIZE];

    uint16_t scrbuf[TERM_SCRBUF_SIZE];
};

void term_init(void);
void term_set_color(uint8_t color);
void term_set_status(enum term_status status);

void term_scroll(uint8_t count);
void term_put_entry_at(char c, uint8_t color, uint16_t x, uint16_t y);
void term_on_input(uint8_t cascii);

void term_put_char_at(char c, uint16_t x, uint16_t y);
void term_put_char(char c);

void term_newline(void);
void term_delete(void);
void term_backspace(void);
void term_cls(void);

void term_write_string_at(uint16_t x, uint16_t y, uint8_t color, const char* str);
void term_write_format_at(uint16_t x, uint16_t y, uint8_t color, const char* format, ...);

void term_write_status_string(const char* str);
void term_write_status_format(const char* format, ...);
void term_clear_status(void);

void term_panic(const char* str);

void term_set_cursor_width(uint8_t begin_height, uint8_t end_height);
void term_enable_cursor(bool enabled);

uint16_t term_wherex(void);
uint16_t term_wherey(void);
void term_gotoxy(uint16_t x, uint16_t y);

void term_print_string(const char* str);
void term_print_format(const char* format, ...);

uint8_t term_get_cascii(void);
size_t term_getline(char* buf, size_t size);
char term_getchar(void);

void term_clear_line_buffer(void);
void term_clear_input_buffer(void);
void term_clear_hitkey_buffer(void);
void term_clear_all_buffers(void);

bool term_hitkey_exists(void);
