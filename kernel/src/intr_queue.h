#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "pic.h"

enum
{
    INTR_QUEUE_FLAG_KEYBOARD = PIC_IRQ_KEYBOARD << 24,

    INTR_QUEUE_MASK_FLAG = 0xff000000,
    INTR_QUEUE_MASK_DATA = 0x00ffffff,
};

void intr_queue_init(void);
void intr_queue_push(uint32_t value);
bool intr_queue_pop_syslock(uint32_t* value);
