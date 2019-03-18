.extern reads, lba_packet
.extern fat12_detect

.global fs_init
.global fs_read
.global bootpart_lba

.set lba_packet.count, 2
.set lba_packet.addr, 4
.set lba_packet.add1, 6
.set lba_packet.lba, 8
.set lba_packet.lba1, 10
.set lba_packet.lba2, 12
.set lba_packet.lba3, 14

.set mbr.part 446
.set part.lba 8
.set part.sectors 12

.section .stage, "awx", @progbits
.code16

fs_read:
    jmp *pfn_fs_read

fs_init:
    push %bx
    push %cx
    push %si
    push %di
    push %bp
    push %es
    cld

    mov $0x1000, %ax
    mov %ax, %es
    mov $lba_packet, %bp

    // read mbr
    movw $1, lba_packet.count(%bp)
    movw $0, lba_packet.addr(%bp)
    movw $1, lba_packet.add1(%bp)
    movw $0, lba_packet.lba(%bp)
    movw $0, lba_packet.lba1(%bp)
    movw $0, lba_packet.lba2(%bp)
    movw $0, lba_packet.lba3(%bp)
    call reads
    mov $.read_error, %si
    jc panic16

    mov $.conf_error, %si
    mov $mbr.part, %di
    movzx .boot_part, %ax
    test %ax, %ax
    jz .nopart

    dec %ax
    shl $4, %ax
    add %ax, %di

    mov %es:part.lba(%di), %eax
    test %eax, %eax
    jz panic16
    mov %eax, bootpart_lba
    jmp 0f

.nopart:
    mov $16 * 4, %cx
    xor %ax, %ax
    repe scasw
    jne panic16
0:

    mov $.part_error, %si
    call fat12_detect
    jc panic16

    xor %ax, %ax
    jmp 0f
.read_error:
    mov $1f, %ax
    jmp 0f
.conf_error:
    mov $2f, %ax
    jmp 0f
.part_error:
    mov $3f, %ax
    jmp 0f
0:
    pop %es
    pop %bp
    pop %di
    pop %si
    pop %cx
    pop %bx
    ret

.section .sdata, "aw", @progbits

1: .asciz "Cannot read media"
2: .asciz "Bootloader misconfigured"
3: .asciz "Cannot recognize partition type"

.section .sbss, "aw", @nobits

.lcomm bootpart_lba, 8
.lcomm pfn_fs_read, 2
.lcomm bootpart, 1
