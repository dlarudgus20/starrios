#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct array8
{
    uint8_t* buf;
    uint8_t* now;
    uint8_t* end;
};

void array8_init(struct array8* ar, uint8_t* buf, uint8_t* end);
size_t array8_max_size(struct array8* ar);
size_t array8_size(struct array8* ar);
bool array8_insert(struct array8* ar, uint8_t* pos, uint8_t data);
void array8_erase(struct array8* ar, uint8_t* pos);
void array8_append(struct array8* dest, const struct array8* src);
bool array8_pushback(struct array8* ar, uint8_t data);
uint8_t array8_popback(struct array8* ar);
