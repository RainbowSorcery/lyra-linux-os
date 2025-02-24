#include "../include/device/mouse.h"
#include "../../common/cpu_instr.h"
#include "../include/irq.h"

// 等待键盘控制器准备完成 由于cpu初始化比键盘初始化快，如果在键盘没初始化好 cpu就发指令容易依法错误
void wait_kbc_send_ready()
{
    while (1)
    {
        unit8_t b = inb(PORT_KEYCMD) & inb(KEySTA_SEND_NOTREADY);

        if (b == 0)
        {
            break;
        }
    }
}

void enable_mouse()
{
    wait_kbc_send_ready();

    outb(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
    wait_kbc_send_ready();
    outb(PORT_KEYDAT, MOUSECMD_ENABLE);
}

void mouse_init()
{
    // 初始化键盘控制电路
    wait_kbc_send_ready();
    outb(PORT_KEYCMD, KEYCMD_WRITE_MODE);
    wait_kbc_send_ready();
    outb(PORT_KEYDAT, KBC_MODE);

    // enable_mouse();

    // irq_install(0x2c, (irq_handler_t)exception_handler_mouse);

    // irq_enable(0x2c);
}

void do_handler_mouse(exception_frame_t *frame)
{
    outb(PIC1_OCW2, 0x64);
    outb(PIC0_OCW2, 0x62);

    unit8_t data = inb(PORT_KEYDAT);
}
