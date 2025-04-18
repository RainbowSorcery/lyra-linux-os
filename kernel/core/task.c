#include "../include/core/task.h"
#include "../include/tools/klib.h"
#include "../include/os_cfg.h"
#include "../include/cpu.h"
#include "../tools/log.h"
#include "../../common/cpu_instr.h"

// 任务管理器
static task_managemnet_t task_managment;


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

    task->time_ticks = 10;
    task->slice_ticks = task->time_ticks;

    return 0;
}

int task_init(task_t *task, unint32_t entry, unint32_t esp, char* name)
{
    tss_init(task, entry, esp);

    list_node_init(&task->run_node);
    list_node_init(&task->all_node);

    kernel_strcpy_size(task->name, name, 32);
    task->state = CREATE;

    // 将任务添加到就绪队列中
    task_set_ready(task);

    list_last_insert(&task_managment.task_list, &task->all_node);

    return 0;
}


void task_set_ready(task_t *task) {
    list_last_insert(&task_managment.ready_list, &task->run_node);
    task->state = READY;
}

void task_set_block(task_t *task) {
    list_remove(&task_managment.ready_list, &task->run_node);
}


// 初始化任务管理器
void init_task_managment() 
{
    list_init(&task_managment.ready_list);
    list_init(&task_managment.task_list);
    task_managment.current_task = 0;
}

// 首个任务初始化 需要设置第一个任务的任务段 tr寄存器 才能进行下一个任务的切换
void task_first_init()
{
    task_init(&task_managment.first_task, 0, 0, "first task");
    write_tr(task_managment.first_task.tss_sel);
    task_managment.current_task = &task_managment.first_task;
}

// 获取首个任务
task_t *task_first_task()
{
    return &task_managment.first_task;
}

int sys_sched_yaied(void)
{
    // 任务的调度流程为在进行进程切换的时候，将当前还未运行完毕的进程从就绪队列首部移除，添加到就绪队列尾部，然后将当前运行的进程切换为就绪队列的首部的进程
    
    // 如果就绪队列当前只有一个任务，那么直接让这个任务一直占用cpu就行了，就没必要进行进程切换了
    if (list_count(&task_managment.ready_list) > 1) {
        task_current()->state = READY;
        list_remove(&task_managment.ready_list, &task_managment.current_task->run_node);
        list_last_insert(&task_managment.ready_list, &task_managment.current_task->run_node);

        // 进行进程切换
        task_dispach();
    }
}


task_t* task_next_run() 
{
    list_node_t *next_task_node = list_first(&task_managment.ready_list);

    return list_node_parent(next_task_node, task_t, run_node);
}

void task_dispach() 
{
    task_t *from_task = task_managment.current_task;

    task_t* to_task = task_next_run();

    to_task->state = RUNNING;
    task_managment.current_task = to_task;

    switch_to_tss(from_task, to_task);
}

task_t *task_current()
{
    return task_managment.current_task;
}


void switch_to_tss(task_t* from, task_t* to)
{
    log_printf("Preparing to switch processes. Current process name: %s, Target process name: %s", from->name, to->name);
    far_jump(to->tss_sel, 0);
}

void task_time_tick() 
{
    // 获取当前进程时间片
    task_t * current_task = task_current();
    // 时间片建议
    current_task->slice_ticks--;
    // 判断时间片是否到0 如果到0则进行进程切换
    if (current_task->slice_ticks <= 0)
    {
        current_task->slice_ticks = current_task->time_ticks;
        task_dispach();
    }
}