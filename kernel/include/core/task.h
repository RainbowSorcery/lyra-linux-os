#ifndef TASK_H
#define TASK_H

#include "../../../common/types.h"
#include "../cpu.h"
#include "../tools/list.h"

typedef struct _task_t
{
    task_state_segemtn tss;
    int tss_sel;
} task_t;

int task_init(task_t *task, unint32_t entry, unint32_t esp);

// 任务切换
void switch_to_tss(task_t *from, task_t *to);

// 任务管理器
typedef struct _task_managemnet_t
{
    // 就绪进行队列
    list_t ready_list;
    // 所有进程队列
    list_t task_list;

    // 当前正在运行的任务
    task_t *current_task;

    // 首个任务
    task_t first_task;
} task_managemnet_t;

// 初始化任务管理器
void init_task_managment();

// 首个任务初始化
void task_first_init();

// 获取首个任务
task_t *task_first_task();

#endif