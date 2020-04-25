#pragma once

#include <stddef.h>
#include <stdint.h>

enum
{
    PIC_MASTER_PORT1 = 0x20,
    PIC_MASTER_PORT2 = 0x21,
    PIC_SLAVE_PORT1 = 0xa0,
    PIC_SLAVE_PORT2 = 0xa1,

    PIC_INTERRUPT_NUMBER = 0x20
};

enum
{
    PIC_IRQ_TIMER               = 0,
    PIC_IRQ_KEYBOARD            = 1,
    PIC_IRQ_SLAVE               = 2,
    PIC_IRQ_SERIAL1             = 3,
    PIC_IRQ_SERIAL2             = 4,
    PIC_IRQ_PARALLEL1           = 5,
    PIC_IRQ_FLOPPY              = 6,
    PIC_IRQ_PARALLEL2           = 7,
    PIC_IRQ_RTC                 = 8,
    PIC_IRQ_MOUSE               = 12,
    PIC_IRQ_COPROCESSER         = 13,
    PIC_IRQ_HARDDISK1           = 14,
    PIC_IRQ_HARDDISK2           = 15
};
enum
{
    PIC_MASKBIT_TIMER           = 1 << PIC_IRQ_TIMER,
    PIC_MASKBIT_KEYBOARD        = 1 << PIC_IRQ_KEYBOARD,
    PIC_MASKBIT_SLAVE           = 1 << PIC_IRQ_SLAVE,
    PIC_MASKBIT_SERIAL1         = 1 << PIC_IRQ_SERIAL1,
    PIC_MASKBIT_SERIAL2         = 1 << PIC_IRQ_SERIAL2,
    PIC_MASKBIT_PARALLEL1       = 1 << PIC_IRQ_PARALLEL1,
    PIC_MASKBIT_FLOPPY          = 1 << PIC_IRQ_FLOPPY,
    PIC_MASKBIT_PARALLEL2       = 1 << PIC_IRQ_PARALLEL2,
    PIC_MASKBIT_RTC             = 1 << PIC_IRQ_RTC,
    PIC_MASKBIT_MOUSE           = 1 << PIC_IRQ_MOUSE,
    PIC_MASKBIT_COPROCESSER     = 1 << PIC_IRQ_COPROCESSER,
    PIC_MASKBIT_HARDDISK1       = 1 << PIC_IRQ_HARDDISK1,
    PIC_MASKBIT_HARDDISK2       = 1 << PIC_IRQ_HARDDISK2
};
enum
{
    PIC_INTERRUPT_TIMER         = PIC_INTERRUPT_NUMBER + PIC_IRQ_TIMER,
    PIC_INTERRUPT_KEYBOARD      = PIC_INTERRUPT_NUMBER + PIC_IRQ_KEYBOARD,
    PIC_INTERRUPT_SLAVE         = PIC_INTERRUPT_NUMBER + PIC_IRQ_SLAVE,
    PIC_INTERRUPT_SERIAL1       = PIC_INTERRUPT_NUMBER + PIC_IRQ_SERIAL1,
    PIC_INTERRUPT_SERIAL2       = PIC_INTERRUPT_NUMBER + PIC_IRQ_SERIAL2,
    PIC_INTERRUPT_PARALLEL1     = PIC_INTERRUPT_NUMBER + PIC_IRQ_PARALLEL1,
    PIC_INTERRUPT_FLOPPY        = PIC_INTERRUPT_NUMBER + PIC_IRQ_FLOPPY,
    PIC_INTERRUPT_PARALLEL2     = PIC_INTERRUPT_NUMBER + PIC_IRQ_PARALLEL2,
    PIC_INTERRUPT_RTC           = PIC_INTERRUPT_NUMBER + PIC_IRQ_RTC,
    PIC_INTERRUPT_MOUSE         = PIC_INTERRUPT_NUMBER + PIC_IRQ_MOUSE,
    PIC_INTERRUPT_COPROCESSER   = PIC_INTERRUPT_NUMBER + PIC_IRQ_COPROCESSER,
    PIC_INTERRUPT_HARDDISK1     = PIC_INTERRUPT_NUMBER + PIC_IRQ_HARDDISK1,
    PIC_INTERRUPT_HARDDISK2     = PIC_INTERRUPT_NUMBER + PIC_IRQ_HARDDISK2
};

/**
 * @brief PIC를 초기화합니다.
 */
void pic_init(void);

/**
 * @brief PIC 인터럽트를 마스크합니다.
 * @param[in] mask 마스크 비트입니다.
 */
void pic_set_mask(uint16_t mask);

/**
 * @brief 특정 IRQ에 EOI를 보냅니다.
 * @param[in] irq EOI를 보낼 IRQ 번호입니다.
 */
void pic_send_eoi(uint8_t irq);
