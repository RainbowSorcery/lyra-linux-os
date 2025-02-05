
#define GDT_TABLE

#include "../include/os_cfg.h"
#include "../include/cpu.h"
#include "../common/cpu_instr.h"
#include "../include/irq.h"
#include "../include/device/time.h"

static segment_desc_t gdt_table[GDT_TABLE_SIZE];

void segement_desc_set(int selector, unint32_t limit, unint32_t base, uint16_t attr)
{
    // selector表示偏移量 从gdt表开始到偏移的字节数 传参传进来的是*8，我们只需要gdt_table的下标数所有需要除8

    // 判断limit是否超过了20位，那么将G标志位设置成1表述以4KB为单位
    if (limit > 0xfffff)
    {
        limit = limit / 0x1000;
        attr = attr | SEG_G;
    }

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

    segement_desc_set(KENEL_SECTION_DS, 0Xffffffff, 0, SEG_P | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_DATA | SEG_TYPE_RW | SEG_D);
    segement_desc_set(KENEL_SECTION_CS, 0Xffffffff, 0, SEG_P | SEG_DPL0 | SEG_S_NORMAL | SEG_TYPE_CODE | SEG_TYPE_RW | SEG_D);
    lgdt((unint32_t)gdt_table, sizeof(gdt_table));
}

void cpu_init()
{
    init_gdt();
    irq_init();
    time_init();
}