.section .text
.code64

// rdi: dest, rsi: ch, rdx: count
// return: dest
.global memset
memset:
    test %rdx, %rdx
    jz 0f

    mov %esi, %eax
    mov %al, %ah
    movzx %ax, %esi
    shl $16, %eax
    or %esi, %eax
    mov %eax, %esi
    shl $32, %rax
    mov %esi, %eax

    mov %rdx, %rcx
    mov %rdi, %r8
    shr $3, %rcx
    rep stosq
    mov %rdx, %rcx
    and $7, %rcx
    rep stosb
    mov %r8, %rax
    ret
0:
    mov %rdi, %rax
    ret

// rdi: dest, rsi: src, rdx: count
// return: dest
.global memcpy
memcpy:
    mov %rdi, %rax
    test %rdx, %rdx
    jz 0f
    mov %rdx, %rcx
    shr $3, %rcx
    rep movsq
    mov %rdx, %rcx
    and $7, %rcx
    rep movsb
0:
    ret

// rdi: dest, rsi: src, rdx: count
// return: dest
.global memmove
memmove:
    cmp %rdi, %rsi
    jge memcpy
    mov %rdi, %rax
    test %rdx, %rdx
    jz 0f
    std
    mov %rdx, %rcx
    dec %rdx
    add %rdi, %rdx
    add %rsi, %rdx
    shr $3, %rcx
    rep movsq
    inc %rdx
    and $7, %rdx
    mov %rdx, %rcx
    rep movsb
    cld
0:
    ret
