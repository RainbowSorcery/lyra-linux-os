#include "types.h"

//  为了避免栈空间的浪费，内联函数会在在调用时将内联函数的函数体内容移动到调用的地方在进行编译
static inline void cli(void)
{
    // 关中断
    asm("cli");
}

static inline void sti(void)
{
    // 开中断
    asm("sti");
}

// 读端口
static inline unit8_t inb(uint16_t port)
{
    unit8_t rv;

    asm("inb %[p], %%al" : "=a"(rv) : [p] "d"(port));

    return rv;
}

static inline void outb(uint16_t port, unit8_t data)
{
    // outb al, dx
    asm("outb %[v], %[p]" ::[v] "a"(data), [p] "d"(port));
}

static inline void lgdt(unint32_t start, uint16_t size)
{
    struct gdt
    {
        uint16_t limit;
        unint32_t start_l;
        unint32_t statt_h;
    } gdt;

    gdt.statt_h = start >> 16;
    gdt.start_l = start & 0xffff;
    gdt.limit = size;

    asm("lgdt %[g]" ::[g] "m"(gdt));
}