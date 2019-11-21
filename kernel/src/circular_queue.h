#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct circular_queue8
{
    uint8_t* buf;
    uint8_t* first;
    uint8_t* last;
    size_t size;
    bool empty;
};

void circular_queue8_init(struct circular_queue8* queue, uint8_t* buf, size_t size);
bool circular_queue8_put(struct circular_queue8* queue, uint8_t data);
uint8_t circular_queue8_get(struct circular_queue8* queue, bool peek, bool* success);
bool circular_queue8_remove(struct circular_queue8* queue);

