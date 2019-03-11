OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
    .boot 0x7c00 :
    {
        *(.boot)
    }

    .stage 0x8000 :
    {
        __stage_start = .;
        *(.stage)
        __stage_end = .;
    }
    __stage_sectors = (__stage_end - __stage_start + 0x1ff) / 0x200;
}
