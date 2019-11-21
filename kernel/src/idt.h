#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum
{
    IDT_TABLE_COUNT = 256,

    IDT_TYPE_INTERRUPT = 0xe,
    IDT_TYPE_TRAP = 0xf,
};

struct idt
{
    uint16_t addr_0_15;
    uint16_t segment;

    unsigned IST:3, reserved1:5;
    unsigned type:4, reserved2:1;
    unsigned DPL:2, P:1;

    uint16_t addr_16_31;
    uint32_t addr_32_63;
    uint32_t reserved3;
};

extern struct idt g_idt_table[IDT_TABLE_COUNT];

void idt_table_init(void);
void idt_init(struct idt* idt, uint16_t segment, void* addr);
