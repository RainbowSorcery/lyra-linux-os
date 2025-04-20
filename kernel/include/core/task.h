#ifndef TASK_H
#define TASK_H

#include "../../../common/types.h"
#include "../cpu.h"
#include "../tools/list.h"

typedef struct _task_t
{
    // 就绪队列节点
    list_node_t run_node;

    // 所有进程队列节点
    list_node_t all_node;

    // 任务状态
    enum {
        // 创建
        CREATE,
        // 正在运行
        RUNNING,
        // 睡眠
        SLEEP,
        // 准备好了
        READY,
        // 等待
        WAIITING
    }state;
    // 任务名称
    char name[32];
    task_state_segemtn tss;

    // 当slice_ticks为0时进行进程切换
    unint32_t slice_ticks;

    // 总的时间片数 用于初始化slice_ticks
    unint32_t time_ticks;

    // 休眠时间片 
    unint32_t sleep_ticks;

    int tss_sel;
} task_t;

int task_init(task_t *task, unint32_t entry, unint32_t esp, char* name);

// 任务切换
void switch_to_tss(task_t *from, task_t *to);

// 任务管理器
typedef struct _task_managemnet_t
{
    // 就绪进行队列
    list_t ready_list;
    // 所有进程队列
    list_t task_list;

    // 休眠队列
    list_t sleep_list;

    // 当前正在运行的任务
    task_t *current_task;

    // 空闲进程 当所有进程都进行休眠了，那么就跳到这个空闲进程进行执行
    task_t idle_task;

    // 首个任务
    task_t first_task;
} task_managemnet_t;

// 初始化任务管理器
void init_task_managment();

// 首个任务初始化
void task_first_init();

// 获取首个任务
task_t *task_first_task();

void task_set_ready(task_t *task);

// 任务调度切换
int sys_sched_yaied(void);

task_t *task_current();

void task_dispach();

// 进程切换
void switch_to_tss(task_t *from, task_t *to);

// 时间片轮转
void task_time_tick();

// 将进程添加到休眠队列中
void task_set_sleep(task_t *task, unint32_t ticks);

// 将进程从休眠队列中移除
void task_set_wakeup(task_t *task);

// 进行休眠
void sys_sleep(unint32_t ms);

// 如果所有进程都睡眠了，那么就跳到这个空闲进程里，避免进程切换错误
void idle_task_entry();

#endif