#include "interrupt.h"
#include "terminal.h"
#include "string.h"

static void except_panic(struct interrupt_context* rsp, const char* msg, const char* errdesc)
{
    char buf[1024];
    snprintf(buf, sizeof(buf),
        "%s Exception occur!!!\n"
        "cs(%p) rip(%p) rflag(%p) ss2(%p) rsp2(%p) "
        "rax(%p) rbx(%p) rcx(%p) rdx(%p) rsi(%p) rdi(%p) rbp(%p) "
        "r8(%p) r9(%p) r10(%p) r11(%p) r12(%p) r13(%p) r14(%p) r15(%p) "
        "errcode(%s%p)",
        msg, rsp->cs, rsp->rip, rsp->rflag, rsp->ss2, rsp->rsp2,
        rsp->rax, rsp->rbx, rsp->rcx, rsp->rdx, rsp->rsi, rsp->rdi, rsp->rbp,
        rsp->r8, rsp->r9, rsp->r10, rsp->r11, rsp->r12, rsp->r13, rsp->r14, rsp->r15,
        errdesc, rsp->errcode);
    term_panic(buf);
}

void intc_unknown(struct interrupt_context* rsp)
{
    except_panic(rsp, "unknown interrupt", "");
}

void intc_except00(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x00] #DE `divide by zero`", "");
}

void intc_except01(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x01] #DB `debug`", "");
}

void intc_except02(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x02] `non-maskable interrupt`", "");
}

void intc_except03(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x03] #BP `breakpoint`", "");
}

void intc_except04(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x04] #OF `overflow`", "");
}

void intc_except05(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x05] #BR `bound range extended`", "");
}

void intc_except06(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x06] #UD `invalid opcode`", "");
}

void intc_except07(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x07] #NM `device not available`", "");
}

void intc_except08(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x08] #DF `double fault`", "");
}

void intc_except09(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x09] `coprocessor segment overrun`", "");
}

void intc_except0a(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x0a] #TS `invalid TSS`", "");
}

void intc_except0b(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x0b] #NP `segment not present`", "");
}

void intc_except0c(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x0c] #SS `stack segment fault`", "");
}

void intc_except0d(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x0d] #GP `general protection fault`", "");
}

void intc_except0e(struct interrupt_context* rsp)
{
    static char errdesc[256];

    if (rsp->errcode & 0x1)
        strcpy(errdesc, "page-protection, ");
    else
        strcpy(errdesc, "non-present, ");

    if (rsp->errcode & 0x2)
        strcpy(errdesc, "during writing, ");
    else
        strcat(errdesc, "during reading, ");

    if (rsp->errcode & 0x4)
        strcat(errdesc, "by user, ");

    if (rsp->errcode & 0x8)
        strcat(errdesc, "read from reserved field, ");

    if (rsp->errcode & 0x10)
        strcat(errdesc, "instruction fetch, ");

    strcpy(errdesc + strlen(errdesc) - 2, " : ");
    except_panic(rsp, "[0x0e] #PF `page fault`", errdesc);
}

void intc_except0f(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x0f] `?? (reserved) ??`", "");
}

void intc_except10(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x10] #MF `x87 floating-point exception`", "");
}

void intc_except11(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x11] #AC `alignment check`", "");
}

void intc_except12(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x12] #MC `machine check`", "");
}

void intc_except13(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x13] #XF `SIMD floating-point exception`", "");
}

void intc_except14(struct interrupt_context* rsp)
{
    except_panic(rsp, "[0x14] #VE `virtualization exception`", "");
}
