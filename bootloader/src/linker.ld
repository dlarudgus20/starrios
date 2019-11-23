OUTPUT_FORMAT(elf64-x86-64)

SECTIONS
{
    .boot 0x7c00 :
    {
        *(.boot)
    }

    .debug : { *(.debug*) }

    /DISCARD/ :
    {
        *(.text .data .bss)
        *(.rela.got .got .got.plt .iplt .rela.iplt .igot.plt)
    }
}
