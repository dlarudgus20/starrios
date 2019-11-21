#pragma once

#include <stdint.h>

static inline void asm_cli(void)
{
    __asm__ __volatile__ ( "cli" );
}

static inline void asm_sti(void)
{
    __asm__ __volatile__ ( "sti" );
}

static inline void asm_hlt(void)
{
    __asm__ __volatile__ ( "hlt" );
}

static inline uint8_t asm_in8(uint16_t port)
{
    uint8_t ret;
    __asm__ __volatile__ ( "in %1, %0" : "=a"(ret) : "d"(port) );
    return ret;
}

static inline void asm_out8(uint16_t port, uint8_t data)
{
    __asm__ __volatile__ ( "out %0, %1" : : "a"(data), "d"(port) );
}
