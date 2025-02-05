#include "../include/irq.h"
#include "../common/cpu_instr.h"
#include "../include/cpu.h"

#define IDT_SIZE_NR 128

static gate_desc_t idt_table[IDT_SIZE_NR];

// 默认中断处理函数
void exception_handler_unknown(void);

// 0号异常 除0异常
void exception_handler_divide(void);

// 1号异常 debug异常 调试时发生的异常
void exception_handler_debug(void);

// 2号异常 不可屏蔽中断 硬件驱动中断
void exception_handler_nmi(void);

// 3号异常 断点中断 执行INT3指令时发生断点异常 int 3表示软中断 用于调试程序，调用int 3指令可以查看内存 寄存器当前状态
void exception_handler_breakpoint(void);

// 4号异常 溢出异常 当RFLAGS溢出位等于1时会出现这个异常
void exception_handler_overflow(void);

// 5号异常 数组越界异常
void exception_handler_bound_range(void);

// 6号异常 无效操作码 无效操作指令
void exception_handler_invalid_opcode(void);

// 7号异常 设备不可用
void exception_handler_device_not_available(void);

// 8号异常 双重异常 在处理异常时又出现了异常会出现这个异常
void exception_handler_double_fault(void);

// 9号异常 协处理器段溢出
void exception_handler_coprocessor_segment_overrun(void);

// 10号异常 无效TSS
void exception_handler_invalid_tss(void);

// 11号异常 段不存在
void exception_handler_segment_not_present(void);

// 12号异常 堆栈故障
void exception_handler_stack_fault(void);

// 13号异常 一般保护故障 例如去写只有读权限的段数据
void exception_handler_general_protection(void);

// 14号异常 页错误
void exception_handler_page_fault(void);

// 16号异常 x87浮点异常
void exception_handler_fpu_error(void);

// 17号异常 对齐检查
void exception_handler_alignment_check(void);

// 18号异常 机器检查 当出现内存错误 总线错误时会抛出这个异常
void exception_handler_machine_check(void);

// 19号异常 SIMD浮点异常
void exception_handler_simd_floating_point(void);

// 20号异常 虚拟化异常
void exception_handler_virtualization(void);

// 21号异常 控制保护异常
void exception_handler_control_protection(void);

static void do_default_handler(const char *msg, exception_frame_t *frame)
{
    for (;;)
    {
        hlt();
    }
}

// 默认中断处理函数
void do_handler_unknown(exception_frame_t *frame)
{
    do_default_handler("unkown exception", frame);
}

void do_handler_divide(exception_frame_t *frame)
{
    // do_default_handler("drivide exception", frame);
    int a = 0212;
}

void do_handler_debug(exception_frame_t *frame)
{
    do_default_handler("debug exception", frame);
}

void do_handler_nmi(exception_frame_t *frame)
{
    do_default_handler("nmi exception", frame);
}

void do_handler_breakpoint(exception_frame_t *frame)
{
    do_default_handler("breakpoint exception", frame);
}

void do_handler_overflow(exception_frame_t *frame)
{
    do_default_handler("overflow exception", frame);
}

void do_handler_bound_range(exception_frame_t *frame)
{
    do_default_handler("bound range exception", frame);
}

void do_handler_invalid_opcode(exception_frame_t *frame)
{
    do_default_handler("invalid opcode", frame);
}

void do_handler_device_not_available(exception_frame_t *frame)
{
    do_default_handler("device not available exception", frame);
}

void do_handler_double_fault(exception_frame_t *frame)
{
    do_default_handler("double fault exception", frame);
}

void do_handler_coprocessor_segment_overrun(exception_frame_t *frame)
{
    do_default_handler("coprocessor segment overrun exception", frame);
}

void do_handler_invalid_tss(exception_frame_t *frame)
{
    do_default_handler("invalid tss exception", frame);
}

void do_handler_segment_not_present(exception_frame_t *frame)
{
    do_default_handler("segment not present exception", frame);
}

void do_handler_stack_fault(exception_frame_t *frame)
{
    do_default_handler("stack fault exception", frame);
}

void do_handler_general_protection(exception_frame_t *frame)
{
    do_default_handler("general protection exception", frame);
}

void do_handler_page_fault(exception_frame_t *frame)
{
    do_default_handler("page fault exception", frame);
}

void do_handler_fpu_error(exception_frame_t *frame)
{
    do_default_handler("fpu error exception", frame);
}

void do_handler_alignment_check(exception_frame_t *frame)
{
    do_default_handler("alignment check exception", frame);
}

void do_handler_machine_check(exception_frame_t *frame)
{
    do_default_handler("machine check exception", frame);
}

void do_handler_simd_floating_point(exception_frame_t *frame)
{
    do_default_handler("simd floating point exception", frame);
}

void do_handler_virtualization(exception_frame_t *frame)
{
    do_default_handler("virtualization exception", frame);
}

void do_handler_control_protection(exception_frame_t *frame)
{
    do_default_handler("control protection exception", frame);
}

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
        gate_desc_set(idt_table + i, KENEL_SECTION_CS, (unint32_t)exception_handler_unknown,
                      GATE_P | GATE_DPL0 | GATE_TYPE_INTERRRUPT);
    }

    irq_install(IRQ0_DE, (irq_handler_t)exception_handler_divide);
    irq_install(IRQ1_DB, (irq_handler_t)exception_handler_debug);
    irq_install(IRQ2, (irq_handler_t)exception_handler_nmi);
    irq_install(IRQ3_BP, (irq_handler_t)exception_handler_breakpoint);
    irq_install(IRQ4_OF, (irq_handler_t)exception_handler_overflow);
    irq_install(IRQ5_BR, (irq_handler_t)exception_handler_bound_range);
    irq_install(IRQ6_UD, (irq_handler_t)exception_handler_invalid_opcode);
    irq_install(IRQ7_NM, (irq_handler_t)exception_handler_device_not_available);
    irq_install(IRQ8_DF, (irq_handler_t)exception_handler_double_fault);
    irq_install(IRQ9, (irq_handler_t)exception_handler_coprocessor_segment_overrun);
    irq_install(IRQ10_TS, (irq_handler_t)exception_handler_invalid_tss);
    irq_install(IRQ11_NP, (irq_handler_t)exception_handler_segment_not_present);
    irq_install(IRQ12_SS, (irq_handler_t)exception_handler_stack_fault);
    irq_install(IRQ13_GP, (irq_handler_t)exception_handler_general_protection);
    irq_install(IRQ14_PF, (irq_handler_t)exception_handler_page_fault);
    irq_install(IRQ16_MF, (irq_handler_t)exception_handler_fpu_error);
    irq_install(IRQ17_AC, (irq_handler_t)exception_handler_alignment_check);
    irq_install(IRQ18_MC, (irq_handler_t)exception_handler_machine_check);
    irq_install(IRQ19_XM, (irq_handler_t)exception_handler_simd_floating_point);
    irq_install(IRQ20_VE, (irq_handler_t)exception_handler_virtualization);
    irq_install(IRQ21_CP, (irq_handler_t)exception_handler_control_protection);

    lidt((unint32_t)idt_table, sizeof(idt_table));

    // 初始化pic芯片
    init_pic();

    irq_enable(0x20);
}

void init_pic(void)
{
    // IMR为中断屏蔽寄存器，该寄存器有8位，分别对应每一路IRQ信号，如果值为1，则屏蔽对应的信号
    outb(PIC0_IMR, 0xff);
    outb(PIC1_IMR, 0xff);

    outb(PIC0_ICW1, 0x11);
    // 0x20 - 0x24对应着PIC0的IRQ0-IRQ7的中断
    outb(PIC0_ICW2, 0x20);
    // 100也就是PIC0的第三位IRQ用于连接从PIC
    outb(PIC0_ICW3, 1 << 2);
    outb(PIC0_ICW4, 0x01);

    outb(PIC1_ICW1, 0x11);
    // 0x28 - 0x30 对应着PIC0的IRQ0-IRQ7的中断
    outb(PIC1_ICW2, 0x28);
    // 0x2当前PIC芯片与主PIC芯片的哪个IRQ连接，0x02也就是PIC0的第三个IRQ
    outb(PIC1_ICW3, 0x02);
    outb(PIC1_ICW4, 0x01);

    // 这里又禁止了一次中断的原因是因为在初始化PIC芯片的过程中会重置中断向量参数所以我们需要再设置一次中断寄存器来保证我们设置的是正确的
    // PIC0用于连接从PIC，io_out8(PIC0_IMR, 0xfb)表示PIC0除了IRQ2以外，全部禁止中断
    outb(PIC0_IMR, 0xfb);
    // PIC1全部禁止中断
    outb(PIC1_IMR, 0xff);
}

int irq_install(int iqr_num, irq_handler_t handler)
{
    if (iqr_num >= IDT_SIZE_NR)
    {
        return -1;
    }

    gate_desc_set(idt_table + iqr_num, KENEL_SECTION_CS, (unint32_t)handler,
                  GATE_P | GATE_DPL0 | GATE_TYPE_INTERRRUPT);

    return 0;
}

void irq_enable(int irq_num)
{
    if (irq_num < 0x20)
    {
        return;
    }

    irq_num -= 0x20;
    // 如果irq_num < 8 则表示是PIC0的中断 如果大于8那么是PIC1的中断
    if (irq_num < 8)
    {
        unit8_t mask = inb(PIC0_IMR) & ~(1 << irq_num);
        outb(PIC0_IMR, mask);
        mask = inb(PIC0_IMR) & ~(1 << irq_num);
    }
    else
    {
        unit8_t mask = inb(PIC0_IMR) & ~(1 << irq_num);
        outb(PIC1_IMR, mask);
    }
}

void irq_disable(int irq_num)
{
    if (irq_num < 0x20)
    {
        return;
    }

    irq_num -= 0x20;
    // 如果irq_num < 8 则表示是PIC0的中断 如果大于8那么是PIC1的中断
    if (irq_num < 8)
    {
        unit8_t mask = inb(PIC0_IMR) & ~(0 << irq_num);
        outb(PIC0_IMR, mask);
        mask = inb(PIC0_IMR) & ~(1 << irq_num);
    }
    else
    {
        unit8_t mask = inb(PIC0_IMR) & ~(0 << irq_num);
        outb(PIC1_IMR, mask);
    }
}

// 关中断
void irq_disable_global()
{
    cli();
}

// 开中断
void irq_enable_global()
{
    sti();
}