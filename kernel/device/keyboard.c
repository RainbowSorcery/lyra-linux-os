#include "../../common/cpu_instr.h"
#include "../include/irq.h"
#include "../include/device/keyboard.h"

void do_handler_keyboard(exception_frame_t *frame)
{
    uint16_t data = inb(0x60);
    outb(PIC0_OCW2, 0x61);
}

void init_keyboard()
{
    irq_install(0x21, (irq_handler_t)exception_handler_keyboard);
    // 开启8259a时钟中断
    irq_enable(0x21);
}