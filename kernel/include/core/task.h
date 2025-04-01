#ifndef TASK_H
#define TASK_H

#include "../../../common/types.h"
#include "../cpu.h"


typedef struct _task_t
{
    task_state_segemtn tss;
    int tss_sel;
} task_t;

int task_init(task_t *task, unint32_t entry, unint32_t esp);

// 任务切换
void switch_to_tss(task_t *from, task_t *to);

#endif