#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

enum
{
    KERNEL_CODE_SEG = 0x08,
    KERNEL_DATA_SEG = 0x10,
};

enum
{
    GDT_TABLE_COUNT = 8192,

    GDT_SEGTYPE_DATA_RO = 0x0,
    GDT_SEGTYPE_DATA_RW = 0x2,
    GDT_SEGTYPE_CODE_XO = 0x8,
    GDT_SEGTYPE_CODE_RX = 0xa,
};

struct gdt
{
    uint16_t size_0_15, addr_0_15;
    uint8_t addr_16_23;

    union
    {
        struct
        {
            unsigned segtype:4, S:1, DPL:2, P:1;
        };
        uint8_t type1;
    };

    union
    {
        struct
        {
            unsigned size_16_19:4;
            unsigned A:1, L:1, D:1, G:1;
        };
        uint8_t type2;
    };

    uint8_t addr_24_32;
};

extern struct gdt g_gdt_table[GDT_TABLE_COUNT];

void gdt_table_init(void);
void gdt_init(struct gdt* gdt, uint8_t segtype, uint8_t DPL);
