#ifndef CPU_INSTR_H
#define CPU_INSTR_H

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

static inline uint16_t inw(uint16_t port)
{
    uint16_t rv;

    asm("in %[p], %%ax" : "=a"(rv) : [p] "d"(port));

    return rv;
}

static inline void outb(uint16_t port, unit8_t data)
{
    // outb al, dx
    asm("outb %[v], %[p]" ::[v] "a"(data), [p] "d"(port));
}

static inline void lgdt(unint32_t start, uint16_t size)
{
    struct
    {
        uint16_t limit;
        uint16_t start_l;
        uint16_t statt_h;
    } gdt;

    gdt.statt_h = start >> 16;
    gdt.start_l = start & 0xffff;
    gdt.limit = size - 1;

    asm("lgdt %[g]" ::[g] "m"(gdt));
}

// 读取cr0寄存器的值并返回
static inline unint32_t read_cr0()
{
    unint32_t rv;
    asm("mov %%cr0, %%eax" : "=a"(rv));

    return rv;
}

// 写回cr0寄存器
static inline void write_cr0(unint32_t val)
{
    asm("mov %%eax, %%cr0" ::"a"(val));
}

// 地址跳转
static inline void far_jump(unint32_t selector, unint32_t offset)
{
    unint32_t addr[] = {offset, selector};
    asm("ljmpl *(%[a])" ::[a] "r"(addr));
}

// 加载中断描述符
static inline void lidt(unint32_t start, uint16_t size)
{
    struct
    {
        uint16_t limit;
        uint16_t start_l;
        uint16_t statt_h;
    } idt;

    idt.statt_h = start >> 16;
    idt.start_l = start & 0xffff;
    idt.limit = size - 1;

    asm("lidt %[g]" ::[g] "m"(idt));
}

// 使cpu进入暂停状态
static inline void hlt()
{
    asm("hlt");
}


static inline void write_tr(uint16_t tss_sel) 
{
    asm("ltr %%ax" ::"a"(tss_sel));
}

#endif // CPU_INSTR_H