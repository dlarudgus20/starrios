#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct interrupt_context
{
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t errcode;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflag;
    uint64_t rsp2;
    uint64_t ss2;
};

void int_unknown();
void int_except00();
void int_except01();
void int_except02();
void int_except03();
void int_except04();
void int_except05();
void int_except06();
void int_except07();
void int_except08();
void int_except09();
void int_except0a();
void int_except0b();
void int_except0c();
void int_except0d();
void int_except0e();
void int_except0f();
void int_except10();
void int_except11();
void int_except12();
void int_except13();
void int_except14();
