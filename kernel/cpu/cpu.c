
#define GDT_TABLE

#include "../include/cpu.h"
#include "../include/os_cfg.h"
#include "cpu.h"

static segment_desc_t gdt_table[GDT_TABLE_SIZE];

void segement_desc_set(int selector, unint32_t limit, unint32_t base, uint16_t attr)
{
    // selector表示偏移量 从gdt表开始到偏移的字节数 传参传进来的是*8，我们只需要gdt_table的下标数所有需要除8
    segment_desc_t *desc = gdt_table + (selector >> 3);
    desc->limit_0_15 = limit & 0xffff;
    desc->base_0_15 = base & 0xffff;
    desc->base_16_23 = (base >> 16) & 0xffff;
    desc->base_24_31 = (base >> 24) & 0xff;
    desc->attr = attr | (((limit >> 16) & 0xf) << 8);
    ;
}

void init_gdt()
{
    for (int i = 0; i < GDT_TABLE_SIZE; i++)
    {
        // gdt每个表项占8字节 左移三位表示偏移量乘8
        segement_desc_set(i << 3, 0, 0, 0);
    }
}

void cpu_init()
{
    init_gdt();
}