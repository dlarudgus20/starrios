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

struct array32
{
    uint32_t* buf;
    uint32_t* now;
    uint32_t* end;
};

void array32_init(struct array32* ar, uint32_t* buf, uint32_t* end);
size_t array32_max_size(struct array32* ar);
size_t array32_size(struct array32* ar);
bool array32_insert(struct array32* ar, uint32_t* pos, uint32_t data);
void array32_erase(struct array32* ar, uint32_t* pos);
void array32_append(struct array32* dest, const struct array32* src);
bool array32_pushback(struct array32* ar, uint32_t data);
uint32_t array32_popback(struct array32* ar);
