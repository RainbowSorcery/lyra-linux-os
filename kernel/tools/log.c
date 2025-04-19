#include "../../common/cpu_instr.h"
#include <stdarg.h>
#include "../include/tools/klib.h"
#include "../include/irq.h"
#define PORT 0x3f8

// 串口初始化
void log_init()
{
    outb(PORT + 1, 0x00); // 禁用所有中断
    outb(PORT + 3, 0x80); // 使能DLAB(设置波特率除数)
    outb(PORT + 0, 0x03); // 将除数设置为3 (lo byte) 38400波特
    outb(PORT + 1, 0x00); //                  (hi byte)
    outb(PORT + 3, 0x03); // 8位，无奇偶校验，一个停止位
    outb(PORT + 2, 0xC7); // 开启FIFO，清零，14字节阈值
    outb(PORT + 4, 0x0F);
}

int is_transmit_empty()
{
    return inb(PORT + 5) & 0x20;
}

void log_printf(const char *fmt, ...)
{
    char str_buffer[128];
    va_list args;
    va_start(args, fmt);
    kernel_memset(str_buffer, '\0', sizeof(str_buffer));
    kenerl_vsprintf(str_buffer, fmt, args);
    va_end(args);

    irq_state_t state = irq_enter_protection();
    
    const char *p = str_buffer;

    while (*p != '\0')
    {
        while (is_transmit_empty() == 0)
            ;
        outb(PORT, *p++);
    }

    outb(PORT, '\r');
    outb(PORT, '\n');

    irq_leave_protection(state);
}
