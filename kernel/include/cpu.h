#ifndef CPU_H
#define CPU_H

#include "../common/types.h"

// 禁止在结构体中填充东西
#pragma pack(1)

typedef struct _segment_desc_t
{
    uint16_t limit_0_15;
    uint16_t base_0_15;
    unit8_t base_16_23;
    uint16_t attr;
    unit8_t base_24_31;
} segment_desc_t;

typedef struct _gate_desc_t
{
    uint16_t offset_0_15;
    uint16_t selector;
    uint16_t attr;
    uint16_t offset_16_31;

} gate_desc_t;


// 任务状态段
typedef struct _task_state_segement
{
    unint32_t pre_link;
    unint32_t esp0, ss0, esp1, ss1, esp2, ssp2;
    unint32_t cr3;
    unint32_t eip, eflags, eax,ecx, ebx, esp, ebp, esi, edi;
    unint32_t es, cs, ss, ds, fs, gs;
    unint32_t idt;
    unint32_t iomap;
} task_state_segemtn;


#pragma pack()

// -------------------------段描述符属性----------------------------
#define SEG_G (1 << 15)
#define SEG_D (1 << 14)
#define SEG_P (1 << 7)

#define SEG_DPL0 (0 << 5)
#define SEG_DPL3 (3 << 5)

#define SEG_S_SYSTEM (0 << 4)
#define SEG_S_NORMAL (1 << 4)

#define SEG_TYPE_CODE (1 << 3)
#define SEG_TYPE_DATA (0 << 3)

#define SEG_TYPE_RW (1 << 1)

// ------------------段选择子---------------------------------------
// 代码段必须是8 数据段必须是16 设置反了了会触发13号异常 不知道为什么
#define KENEL_SECTION_CS (1 * 8)
#define KENEL_SECTION_DS (2 * 8)

// ----------------中断描述符属性-------------------------------------
#define GATE_P (1 << 15)
#define GATE_DPL0 (0 << 14)
#define GATE_DPL3 (3 << 13)
#define GATE_TYPE_INTERRRUPT (0xE << 8)
#define GATE_TYPE_TRAP (0xF << 8)

void cpu_init();

#endif // CPU_H