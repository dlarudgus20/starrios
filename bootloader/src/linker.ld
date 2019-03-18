OUTPUT_FORMAT(elf64-x86-64)

MEMORY
{
    BOOTCODE : ORIGIN = 0x7c00, LENGTH = 0x0200
    BOOTBSS : ORIGIN = 0x7e00, LENGTH = 0x0200
    STAGE : ORIGIN = 0x8000, LENGTH = 0x7000
    STAGEBSS : ORIGIN = 0xf000, LENGTH = 0x1000
}

SECTIONS
{
    __page_pml4t = 0x1000;
    __page_pdpt = 0x2000;
    __page_pdt = 0x3000;
    __page_table = 0x4000;

    __stack_top = 0x7000;

    .boot 0x7c00 :
    {
        *(.boot)
    } >BOOTCODE

    .bootbss 0x7e00 :
    {
        *(.bootbss)
    } >BOOTBSS

    .stage 0x8000 :
    {
        __stage_start = .;
        *(.stage)
        *(.sdata)
        __stage_end = .;
    } >STAGE
    __stage_sectors = (__stage_end - __stage_start + 0x1ff) / 0x200;

    .sbss 0xf000 :
    {
        __sbss_start = .;
        *(.sbss)
        __sbss_end = .;
    } >STAGEBSS
    __sbss_size = __sbss_end - __sbss_start;
}
