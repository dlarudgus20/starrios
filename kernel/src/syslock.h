#pragma once

#include <stdint.h>

typedef uint64_t syslock_t;

static inline syslock_t syslock_acquire(void)
{
    uint64_t ret;
    __asm__ __volatile__ ( "pushfq \n pop %0 \n cli" : "=r"(ret) );
    return ret;
}

static inline void syslock_release(syslock_t lock)
{
    __asm__ __volatile__ ( "push %0 \n popfq" : : "r"(lock) );
}
