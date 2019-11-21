#include "gdt.h"
#include "string.h"

struct gdt g_gdt_table[GDT_TABLE_COUNT];

void gdt_table_init(void)
{
    memset(g_gdt_table, 0, sizeof(g_gdt_table));

    gdt_init(g_gdt_table + 1, GDT_SEGTYPE_CODE_XO, 0);
    gdt_init(g_gdt_table + 2, GDT_SEGTYPE_DATA_RW, 0);

    uint64_t gdtr[2] = {
        ((uint64_t)g_gdt_table << 16) | (GDT_TABLE_COUNT * sizeof(struct gdt) - 1),
        (uint64_t)g_gdt_table >> 48
    };
    __asm__ __volatile__
    (
        "lgdt %0            \n"
        "mov $0x10, %%rax   \n"
        "mov %%rax, %%ds    \n"
        "mov %%rax, %%es    \n"
        "mov %%rax, %%fs    \n"
        "mov %%rax, %%gs    \n"
        "movabs $1f, %%rax  \n"
        "pushq $0x08        \n"
        "push %%rax         \n"
        "retfq              \n"
        "1:                 \n"
        : : "m"(*gdtr)
        : "rax"
    );
}

void gdt_init(struct gdt* gdt, uint8_t segtype, uint8_t DPL)
{
    gdt->addr_0_15 = 0;
    gdt->size_0_15 = 0xffff;
    gdt->addr_16_23 = 0x00;
    gdt->segtype = segtype;
    gdt->S = 1;
    gdt->DPL = DPL;
    gdt->P = 1;
    gdt->size_16_19 = 0xf;
    gdt->A = 0;
    gdt->L = 1;
    gdt->D = 0;
    gdt->G = 1;
    gdt->addr_24_32 = 0;
}
