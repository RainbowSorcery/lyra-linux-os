#include "../include/core/task.h"
#include "../include/tools/klib.h"
#include "../include/os_cfg.h"
#include "../include/cpu.h"
#include "../tools/log.h"
#include "../../common/cpu_instr.h"

int tss_init(task_t *task, unint32_t entry, unint32_t esp) 
{
    int tss_sel = gdt_alloc_desc();
    if (tss_sel < 0) {
        log_printf("alloc tss failed.. ");
    }

    segement_desc_set(tss_sel, (unint32_t)sizeof(task_state_segemtn), (unint32_t)&(task)->tss, SEG_P | SEG_DPL0 | SEG_TYPE_TSS);

    kernel_memset(&task->tss, 0, sizeof(task_state_segemtn));
    // 程序执行地址
    task->tss.eip = entry;
    // 程序栈
    task->tss.esp0 = task->tss.esp = esp;
    // 平坦模型 栈段设置成数据段 其他寄存器设置成代码段
    task->tss.ss = task->tss.ss0 = KENEL_SECTION_DS;
    task->tss.es = task->tss.ds = task->tss.fs = task->tss.gs = KENEL_SECTION_DS;
    task->tss.cs = KENEL_SECTION_CS;

    // 根据手册设置eflag寄存器，if位设置成0，避免tss切换后中断无法响应，第二位固定设置成1
    task->tss.eflags = EFLAGS_DEFAULT | EFLAGS_IF;

    task->tss_sel = tss_sel;

    return 0;
}

int task_init(task_t *task, unint32_t entry, unint32_t esp)
{
    tss_init(task, entry, esp);
    return 0;
}
