#include "../common/types.h"
#include "../include/cpu.h"

void irq_init();

void gate_desc_set(gate_desc_t *gdt, uint16_t selector, unint32_t offset, uint16_t attr);

typedef struct _exception_frame_t
{
    unint32_t gs;            // offset 0  → E-56 ✓
    unint32_t fs;            // offset 4  → E-52 ✓
    unint32_t es;            // offset 8  → E-48 ✓
    unint32_t ds;            // offset 12 → E-44 ✓
    unint32_t edi;           // offset 16 → E-40 ✓
    unint32_t esi;           // offset 20 → E-36 ✓
    unint32_t ebp;           // offset 24 → E-32 ✓
    unint32_t esp_old;       // offset 28 → E-28 ✓（pusha保存的原ESP）
    unint32_t ebx;           // offset 32 → E-24 ✓
    unint32_t edx;           // offset 36 → E-20 ✓
    unint32_t ecx;           // offset 40 → E-16 ✓
    unint32_t eax;           // offset 44 → E-12 ✓
    unint32_t exception_num; // offset 48 → E-8  ✓
    unint32_t error_code;    // offset 52 → E    ✓ ← 这就是0x8的来源！
    unint32_t eip;           // offset 56 → E+4  ✓
    unint32_t cs;            // offset 60 → E+8  ✓
    unint32_t eflags;        // offset 64 → E+12 ✓

} exception_frame_t;

typedef void (*irq_handler_t)(exception_frame_t *frame);

int irq_install(int iqr_num, irq_handler_t handler);

#define IRQ0_DE 0
#define IRQ1_DB 1
#define IRQ2 2
#define IRQ3_BP 3
#define IRQ4_OF 4
#define IRQ5_BR 5
#define IRQ6_UD 6
#define IRQ7_NM 7
#define IRQ8_DF 8
#define IRQ9 9
#define IRQ10_TS 10
#define IRQ11_NP 11
#define IRQ12_SS 12
#define IRQ13_GP 13
#define IRQ14_PF 14
#define IRQ16_MF 16
#define IRQ17_AC 17
#define IRQ18_MC 18
#define IRQ19_XM 19
#define IRQ20_VE 20
#define IRQ21_CP 21

// --------------------8259a寄存器--------------------
#define PIC0_ICW1 0x0020
#define PIC0_OCW2 0x0020
#define PIC0_IMR 0x0021
#define PIC0_ICW2 0x0021
#define PIC0_ICW3 0x0021
#define PIC0_ICW4 0x0021
#define PIC1_ICW1 0x00a0
#define PIC1_OCW2 0x00a0
#define PIC1_IMR 0x00a1
#define PIC1_ICW2 0x00a1
#define PIC1_ICW3 0x00a1
#define PIC1_ICW4 0x00a1

#define PIC_OCW2_EOI 0x20

void init_pic();
void irq_enable(int irq_num);
void irq_disable(int irq_num);

void pic_send_eoi(int irq_num);

typedef unint32_t irq_state_t;


// 开启保护临界资源
irq_state_t irq_enter_protection(void);
// 关闭保护临界资源
void irq_leave_protection (irq_state_t state);
