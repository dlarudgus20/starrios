.extern reads, lba_packet
.extern panic16

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

.section .stage, "awx"
.code16

.global part_init
part_init:
    pusha
    push %es

    mov $0x1000, %ax
    mov %ax, %es
    mov $lba_packet, %bp

    movw $1, lba_packet.count(%bp)
    movw $0, lba_packet.addr(%bp)
    movw $1, lba_packet.add1(%bp)
    movw $0, lba_packet.lba(%bp)
    movw $0, lba_packet.lba1(%bp)
    movw $0, lba_packet.lba2(%bp)
    movw $0, lba_packet.lba3(%bp)
    call reads
    jc panic

    popa
    pop %es
    ret
