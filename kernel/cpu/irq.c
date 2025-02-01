#include "../include/irq.h"
#include "../common/cpu_instr.h"
#include "../include/cpu.h"

#define IDT_SIZE_NR 128

static gate_desc_t idt_table[IDT_SIZE_NR];

void gate_desc_set(gate_desc_t *gdt, uint16_t selector, unint32_t offset, uint16_t attr)
{
    gdt->offset_0_15 = offset & 0xffff;
    gdt->offset_16_31 = (offset >> 16) & 0xffff;
    gdt->selector = selector;
    gdt->attr = attr;
}

void irq_init()
{
    for (int i = 0; i < IDT_SIZE_NR; i++)
    {
        gate_desc_set(idt_table + i, 0, 0, 0);
    }
    lidt((unint32_t)idt_table, sizeof(idt_table));
}