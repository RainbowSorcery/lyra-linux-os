#include "../include/device/time.h"
#include "../../common/cpu_instr.h"
#include "../include/irq.h"
#include "../include/core/task.h"

static unint32_t sys_tick;

void do_handler_time(exception_frame_t *frame)
{
    // 告诉8259a中断处理完成 不然无法触发下一次中断 必须先响应中断处理完成才能做具体业务，不然在函数调度的过程中可能打断EOI的执行，导致后续中断无法接收到
    pic_send_eoi(0x20);

    sys_tick++;

    // 执行进程调度
    task_time_tick();

}

void init_pit()
{
    // 时钟计数器 每秒时钟频率有RIT_OSC_FREQ Hz 除1000计算出每毫秒多少时钟频率，最后计算出OS_TICKS_MS毫秒是多少频率
    unint32_t reload_count = RIT_OSC_FREQ / (1000.0 / OS_TICKS_MS);
    outb(PIT_COMMAND_MODE_PORT, PIT_COMMAND_CHANNEL_0 | PIT_COMMAND_ACCESS_MODE | PIT_COMMAND_OPER_MODE);

    outb(PIT_COMMAND_DATA_PORT, (reload_count & 0xff));
    outb(PIT_COMMAND_DATA_PORT, (reload_count >> 8) & 0xff);
    unint32_t port = inb(PIT_COMMAND_DATA_PORT);

    irq_install(0x20, (irq_handler_t)exception_handler_time);
    // 开启8259a时钟中断
    irq_enable(0x20);
}

void pic_send_eoi(int irq_num)
{
    if (irq_num < 0x20)
    {
        return;
    }

    irq_num -= 0x20;
    // 如果irq_num < 8 则表示是PIC0的中断 如果大于8那么是PIC1的中断
    if (irq_num >= 8)
    {
        outb(PIC1_OCW2, PIC_OCW2_EOI);
    }

    outb(PIC0_OCW2, PIC_OCW2_EOI);
}

void time_init()
{
    sys_tick = 0;

    init_pit();
    sti();
}