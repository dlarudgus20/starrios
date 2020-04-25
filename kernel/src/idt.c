#include "idt.h"
#include "gdt.h"
#include "interrupt.h"

struct idt g_idt_table[IDT_TABLE_COUNT];

void idt_table_init(void)
{
    for (unsigned i = 0; i < IDT_TABLE_COUNT; ++i)
    {
        idt_init(g_idt_table + i, KERNEL_CODE_SEG, int_unknown);
    }

    static void (* const handlers[])() = {
        int_except00,
        int_except01,
        int_except02,
        int_except03,
        int_except04,
        int_except05,
        int_except06,
        int_except07,
        int_except08,
        int_except09,
        int_except0a,
        int_except0b,
        int_except0c,
        int_except0d,
        int_except0e,
        int_except0f,
        int_except10,
        int_except11,
        int_except12,
        int_except13,
        int_except14,
    };
    for (unsigned i = 0; i < sizeof(handlers) / sizeof(handlers[0]); ++i)
    {
        idt_init(g_idt_table + i, KERNEL_CODE_SEG, handlers[i]);
    }

    idt_init(g_idt_table + 0, KERNEL_CODE_SEG, int_except00);

    struct {
        uint16_t limit;
        void* base;
    } __attribute__((packed)) idtr;
    idtr.limit = IDT_TABLE_COUNT * sizeof(struct idt) - 1;
    idtr.base = g_idt_table;

    __asm__ __volatile__ ( "lidt %0" : : "m"(idtr) );
}

void idt_init(struct idt* idt, uint16_t segment, void* addr)
{
    idt->addr_0_15 = (uint64_t)addr & 0xffff;
    idt->segment = segment;

    idt->IST = 0;
    idt->reserved1 = 0;
    idt->type = IDT_TYPE_INTERRUPT;
    idt->reserved2 = 0;
    idt->DPL = 0;
    idt->P = 1;

    idt->addr_16_31 = ((uint64_t)addr >> 16) & 0xffff;
    idt->addr_32_63 = (uint64_t)addr >> 32;
    idt->reserved3 = 0;
}
