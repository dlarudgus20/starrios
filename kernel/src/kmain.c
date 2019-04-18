#include "gdt.h"
#include "idt.h"
#include "terminal.h"

void kmain(void)
{
    gdt_table_init();
    idt_table_init();

    __asm__ __volatile__ ( "int $0x42" );

    char *video = (char *)0xffff80001feb8000;
    const char *msg = "hello kmain";

    for (const char* ptr = msg; *ptr != '\0'; ++ptr)
    {
        *video++ = *ptr;
        *video++ = 0x7d;
    }

    while (1) __asm__ __volatile__ ( "hlt" );
}
