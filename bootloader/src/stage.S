.extern __sbss_start, __sbss_size
.extern fs_init

.global stage_entry

.section .stage, "awx", @progbits
.code16

stage_entry:
    cld
    mov $__sbss_start, %di
    mov $__sbss_size / 2, %cx
    xor %ax, %ax
    rep movsw

    call fs_init
    test %ax, %ax
    jnz panic16

    call a20_enable
    test %ax, %ax
    jnz panic16

// ds:si : string to print
print:
    cld
    push %bx
0:
    lodsb
    test %al, %al
    jz 1f
    mov $0x07, %bx
    mov $0x0e, %ah
    int $0x10
    jmp 0b
1:
    pop %bx
    ret

panic16:
    mov %ax, %si
    call print
    sti
0:
    hlt
    jmp 0b

.code32

pm_start:
    mov $0x20, %eax
    mov %eax, %ds
    mov %eax, %es
    mov %eax, %fs
    mov %eax, %gs
    mov %eax, %ss
    mov $0x7000, %esp
    mov %esp, %ebp

    cld
    mov $0xb8000, %edi
    mov $1000, %ecx
    xor %eax, %eax
    repne stosl

    // check if cpuid is suppported
    pushfl
    pop %eax
    mov %eax, %ecx
    xor $1 << 21, %eax
    push %eax
    popfl
    pushfl
    pop %eax
    push %ecx
    popfl
    xor %ecx, %eax
    jz panic32

    // check if long mode is supported
    mov $0x80000000, %eax
    cpuid
    cmp $0x80000001, %eax
    jb panic32
    mov $0x80000001, %eax
    cpuid
    test $1 << 29, %edx
    jz panic32

    // set up paging
    cld
    mov $0x1000, %edi
    mov $1000, %ecx
    xor %eax, %eax
    repnz stosl

    mov $0x1000, %edi
    mov %edi, %cr3
    movl $0x2003, (%edi)
    add $0x1000, %edi
    movl $0x3003, (%edi)
    add $0x1000, %edi
    movl $0x4003, (%edi)
    add $0x1000, %edi

    mov $3, %ebx
    mov $0x200, %ecx
0:
    mov %ebx, (%edi)
    add $0x1000, %ebx
    add $8, %edi
    dec %ecx
    jnz 0b

    mov %cr4, %eax
    or $1 << 5, %eax
    mov %eax, %cr4

    // enter long mode
    mov $0xc0000080, %ecx
    rdmsr
    or $1 << 8, %eax
    wrmsr

    mov %cr0, %eax
    or $1 << 31, %eax
    mov %eax, %cr0

    ljmp $0x08, $lm_start

panic32:
    mov $p32_msg, %esi
    mov $0xb8000, %edi
    mov $0x07, %ah
    cld
0:
    lodsb
    test %al, %al
    jz 1f
    stosw
    jmp 0b
1:
    hlt
    jmp 1b

.code64

lm_start:
    mov $0x10, %rax
    mov %rax, %ds
    mov %rax, %es
    mov %rax, %fs
    mov %rax, %gs
    mov %rax, %ss
    mov $0x7000, %rsp

    mov $lm_greet, %rsi
    mov $0xb8000, %rdi
    mov $0x07, %ah
    cld
0:
    lodsb
    test %al, %al
    jz 1f
    stosw
    jmp 0b
1:
    hlt
    jmp 1b

.section .sdata, "aw", @progbits

p32_msg: .asciz "boot sequence failed... system panic."
lm_greet: .asciz "welcome to 64bit long mode."

gdtr:
    .word gdt_end - gdt - 1
    .long gdt
gdt:
    .long 0, 0
    // 64bit code segment
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b10101111
    .byte 0x00
    // 64bit data segment
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b10101111
    .byte 0x00
    // 32bit code segment
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10011010
    .byte 0b11001111
    .byte 0x00
    // 32bit data segment
    .word 0xffff
    .word 0x0000
    .byte 0x00
    .byte 0b10010010
    .byte 0b11001111
    .byte 0x00
gdt_end:
