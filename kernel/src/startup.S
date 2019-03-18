.extern kmain
.extern __bss_start, __bss_size

.section .startup, "ax"

.code64

.global startup
startup:

    cld
    mov $__bss_start, %rdi
    mov $__bss_size, %rcx
    shr $3, %rcx
    xor %rax, %rax
    repnz stosq

    call kmain
0:
    hlt
    jmp 0b
