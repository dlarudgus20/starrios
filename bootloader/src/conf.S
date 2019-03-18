.extern reads, lba_packet
.extern panic16

.extern fs_init, fs_read

.set lba_packet.count, 2
.set lba_packet.addr, 4
.set lba_packet.add1, 6
.set lba_packet.lba, 8
.set lba_packet.lba1, 10
.set lba_packet.lba2, 12
.set lba_packet.lba3, 14

.section .stage, "awx", @progbits
.code16

conf_init:
    pusha
    push %es
    cld

    mov $conf_script_lba, %bx
    mov 0(%bx), %cx
    mov 2(%bx), %dx
    mov $lba_packet, %bp
    xor %ax, %ax
    mov %ax, %es
    mov $0xfe00, %di

    movw %ax, lba_packet.count(%bp)
    movw %di, lba_packet.addr(%bp)
    movw %ax, lba_packet.add1(%bp)
    movw %cx, lba_packet.lba(%bp)
    movw %dx, lba_packet.lba1(%bp)
    movw %ax, lba_packet.lba2(%bp)
    movw %ax, lba_packet.lba3(%bp)
    call reads
    jc .reads_fail

    mov %bx, $0xfc00
.parse_loop:
    mov (%di), %al
    inc %di
    cmp '\n, %al
    je .parse_loop
    cmp '[, %al
    jne .parse_fail

    mov %di, 0(%bx)
    mov $0x100, %cx
    mov '], %al
    repe scasb
    jne .parse_fail
    cmp $0x0ff, %cx
    je .parse_end
    xor %al, %al
    stosb

    mov $.chainload, %si
    mov $11, %cx
    repe cmpsb
    test %cx, %cx
    jnz 0f
    mov $0, 2(%bx)

    movzx (%di), %ax
    sub '0, %ax
    cmp $4, %ax
    jae .parse_fail
    mov %ax, 4(%bx)
    jmp 1f
0:
    mov $.loadbin, %si
    mov $9, %cx
    repe cmpsb
    test %cx, %cx
    jnz .parse_fail
    cmp '", %al
    jne .parse_fail

    mov %di, 2(%bx)
    mov $0x100, %cx
    mov '", %al
    repe scasb
    jne .parse_fail
    xor %al, %al
    stosb

1:
    add $6, %bx
    cmp $0xfe00, %bx
    jae .parse_fail
    jmp .parse_loop
.parse_end:

    popa
    pop %es
    ret

.reads_fail:
    mov $.read_error, %si
    jmp panic16
.parse_fail:
    mov $.script_error, %si
    jmp panic16

.loadbin: .ascii "\nloadbin "
.chainload: .ascii "\nchainload "

.read_error: .asciz "Cannot read bootloader configure script file"
.script_error: .asciz "Syntax error on bootloader configure script file"

conf_script_lba: .quad 0
conf_script_name: .asciz "boot.cfg"
