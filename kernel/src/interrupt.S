.section .text
.code64

.macro push_regs
push %rax
push %rbx
push %rcx
push %rdx
push %rsi
push %rdi
push %rbp
push %r8
push %r9
push %r10
push %r11
push %r12
push %r13
push %r14
push %r15
.endm

.macro pop_regs
pop %r15
pop %r14
pop %r13
pop %r12
pop %r11
pop %r10
pop %r9
pop %r8
pop %rbp
pop %rdi
pop %rsi
pop %rdx
pop %rcx
pop %rbx
pop %rax
.endm

.macro make_handler errcode name
.extern intc_\name
.global int_\name
int_\name\():
.if !\errcode
    pushq $0
.endif
    push_regs
    mov %rsp, %rdi
    call intc_\name
    pop_regs
    add $8, %rsp
    iretq
.endm

// CPU Exceptions
make_handler 0 unknown
make_handler 0 except00     // Divide-by-Zero Error
make_handler 0 except01     // Debug Exception
make_handler 0 except02     // Non-Maskable Interrupt
make_handler 0 except03     // Breakpoint
make_handler 0 except04     // Overflow
make_handler 0 except05     // Bound Range Exceeded
make_handler 0 except06     // Invalid Opcode
make_handler 0 except07     // Device Not Available
make_handler 1 except08     // Double Fault
make_handler 0 except09     // Coprocessor Segment Overrun
make_handler 1 except0a     // Invalid TSS
make_handler 1 except0b     // Segment Not Present
make_handler 1 except0c     // Stack Segment Fault
make_handler 1 except0d     // General Protection Fault
make_handler 1 except0e     // Page Fault
make_handler 0 except0f     // (reserved)
make_handler 0 except10     // x87 Floating-point Exception
make_handler 1 except11     // Alignment Check
make_handler 0 except12     // Machine Check
make_handler 0 except13     // SIMD Floating-point Exception
make_handler 0 except14     // Virtualization Exception

// Device Interrupts
make_handler 0 keyboard
