#include "intr_queue.h"
#include "syslock.h"
#include "asm.h"

enum
{
    INTR_BUF_SIZE = 1024
};

static volatile uint32_t s_buffer[INTR_BUF_SIZE];
static volatile uint32_t* volatile s_begin;
static volatile uint32_t* volatile s_end;
static volatile bool s_empty;

void intr_queue_init(void)
{
    s_begin = s_end = s_buffer;
    s_empty = true;
}

void intr_queue_push(uint32_t value)
{
    volatile uint32_t* end = s_end;
    if (s_empty || s_begin != end)
    {
        *end++ = value;
        if (end == s_buffer + INTR_BUF_SIZE)
            end = s_buffer;

        s_end = end;
        s_empty = false;
    }
}

bool intr_queue_pop_syslock(uint32_t* value)
{
    bool ret = false;
    syslock_t syslock = syslock_acquire();

    if (!s_empty)
    {
        volatile uint32_t* beg = s_begin;
        *value = *beg++;
        if (beg == s_buffer + INTR_BUF_SIZE)
            beg = s_buffer;

        s_begin = beg;
        if (beg == s_end)
            s_empty = true;

        ret = true;
    }

    syslock_release(syslock);
    return ret;
}
