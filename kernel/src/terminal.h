#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define term_make_color(fg, bg) (((bg) << 4) | (fg))

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
    TERM_COLOR_STATUS = term_make_color(TERM_WHITE, TERM_LIGHT_GREY),
    TERM_COLOR_INPUT = term_make_color(TERM_WHITE, TERM_BLACK),
    TERM_COLOR_PANIC = term_make_color(TERM_RED, TERM_WHITE),
};

struct term_pos
{
    uint8_t col;
    uint8_t row;
};

void term_init(void);
void term_enable_cursor(bool enable);
void term_set_status_color(uint8_t color);

struct term_pos term_write_string_at(uint8_t color, uint8_t col, uint8_t row, const char* line);
struct term_pos term_write_format_at(uint8_t color, uint8_t col, uint8_t row, const char* format, ...);
void term_write_string(uint8_t color, const char* line);
void term_write_format(uint8_t color, const char* format, ...);

void term_write_status(const char* status);
void term_write_stat_f(const char* format, ...);
void term_panic(const char* message);

void term_mode_normal(void);
void term_mode_input(const char* prompt);
void term_on_input(uint8_t cascii);
