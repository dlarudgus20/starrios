.global a20_enable

.section .stage, "awx", @progbits
.code16

a20_enable:
    pushf
    cli

    call a20_check
    test %ax, %ax
    jnz 0f

    mov $0x2401, %ax
    int $0x15
    jc 1f
    test $0, %ax
    jnz 1f

    call a20_check
    test %ax, %ax
    jz 1f

0:
    xor %ax, %ax
    jmp 2f
1:
    mov $.failmsg, %ax
2:
    popf
    ret

// ax=0 if a20 is disabled; ax=1 if a20 is enabled
a20_check:
    push %ds
    push %es
    push %si
    push %di

    xor %ax, %ax
    mov %ax, %es // es := 0x0000
    not %ax
    mov %ax, %ds // ds := 0xffff
    mov $0x0500, %di
    mov $0x0510, %si

    mov %es:(%di), %dh
    mov %ds:(%si), %dl
    movb $0x00, %es:(%di)
    movb $0xff, %ds:(%si)
    cmpb $0xff, %es:(%di)

    mov %dh, %es:(%di)
    mov %dl, %ds:(%si)
    mov $0, %ax
    je 0f
    mov $1, %ax
0:
    pop %di
    pop %si
    pop %es
    pop %ds
    ret

.section .sdata, "aw", @progbits

.failmsg: .asciz "cannot enable a20 gate"
