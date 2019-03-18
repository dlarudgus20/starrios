.extern reads, lba_packet
.global pfn_fs_read, bootpart_lba
.extern panic16

.global fat12_detect

.set lba_packet.count, 2
.set lba_packet.addr, 4
.set lba_packet.add1, 6
.set lba_packet.lba, 8
.set lba_packet.lba1, 10
.set lba_packet.lba2, 12
.set lba_packet.lba3, 14

.section .stage, "awx"
.code16

fat12_detect:
    pusha
    push %es

    mov $0x1000, %ax
    mov %ax, %es
    mov $lba_packet, %bp

    // read BPB
    mov $bootpart_lba, %bx
    mov 0(%bx), %ax
    mov 2(%bx), %dx
    movw $1, lba_packet.count(%bp)
    movw $0, lba_packet.addr(%bp)
    movw $1, lba_packet.add1(%bp)
    movw %ax, lba_packet.lba(%bp)
    movw %dx, lba_packet.lba1(%bp)
    movw $0, lba_packet.lba2(%bp)
    movw $0, lba_packet.lba3(%bp)
    call reads
    mov $.read_error, %si
    jc panic16



    pop %es
    popa
    ret

.read_error: .asciz "Cannot read media"
.conf_error: .asciz "Bootloader misconfigured"
.part_error: .asciz "Cannot recognize partition type"
