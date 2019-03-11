OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
    .startup 0x00100000 :
    {
        *(.startup)
    }

    __higher_half_displacement = 0xffffffffff000000 - 0x00100000;
    . += __higher_half_displacement;

    .text ALIGN(64) : AT(ADDR(.text) - __higher_half_displacement)
    {
        *(.text .text.*)
    }

    .rodata ALIGN(64) : AT(ADDR(.rodata) - __higher_half_displacement)
    {
        *(.rodata .rodata.*)
    }

    .data ALIGN(64) : AT(ADDR(.data) - __higher_half_displacement)
    {
        *(.data .data.*)
    }

    __bss_start = .;
    .bss ALIGN(64) : AT(ADDR(.bss) - __higher_half_displacement)
    {
        *(COMMON)
        *(.bss .bss.*)
        . = ALIGN(64);
    }
    __bss_end = .;
    __bss_size = __bss_end - __bss_start;
}
