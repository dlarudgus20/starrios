OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
    .startup 0x00100000 :
    {
        *(.startup)
    }

    __higher_half_displacement = 0xffffffffff000000 - 0x00100000;
    . += __higher_half_displacement;

    .text ALIGN(512) : AT(ADDR(.text) - __higher_half_displacement)
    {
        *(.text .stub .text.* .gnu.linkonce.t.*)
    }

    .rodata ALIGN(512) : AT(ADDR(.rodata) - __higher_half_displacement)
    {
        *(.rodata .rodata.* .gnu.linkonce.r.*)
        *(.rodata1)
    }

    .data ALIGN(512) : AT(ADDR(.data) - __higher_half_displacement)
    {
        *(.data .data.* .gnu.linkonce.d.*)
        *(.data1)
    }

    .bss ALIGN(512) : AT(ADDR(.bss) - __higher_half_displacement)
    {
        __bss_start = .;
        *(COMMON)
        *(.bss .bss.* .gnu.linkonce.b.*)
        . = ALIGN(16);
        __bss_end = .;
    }
    __bss_size = __bss_end - __bss_start;
}
