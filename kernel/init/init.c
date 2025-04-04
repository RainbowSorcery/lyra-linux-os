#include "init.h"
#include "../common/boot_info.h"
#include "../include/cpu.h"
#include "../include/tools/list.h"
#include "../tools/log.h"
#include "../include/core/task.h"
#include "../../common/cpu_instr.h"


void kernel_init(boot_info_t *boot_info)
{
    cpu_init();
}

void list_test()
{
    list_t list;
    list_init(&list);

    log_printf("list: first=0x%x, last=0x%x, count=%d", list_first(&list), list_last(&list), list_count(&list));
}

static task_t first_task;
// 设置程序单独的栈避免多个程序共同读写一个栈出现的异常操作
static unint32_t init_task_stack[1024];
static task_t init_task;


void init_task_entry() 
{
    int count = 0;

    for (;;)
    {
        count++;
        log_printf("init task count:%d", count);
        switch_to_tss(&init_task, &first_task);
    }
}

void init_main()
{
    
    int count = 0;

    task_init(&first_task, 0, 0);
    task_init(&init_task, (unint32_t)init_task_entry, (unint32_t)&init_task_stack[1024]);
    write_tr(first_task.tss_sel);

    for (;;)
    {
        switch_to_tss(&first_task, &init_task);
        count++;
        log_printf("init main count:%d", count);
    }
}


void switch_to_tss(task_t* from, task_t* to) 
{
    far_jump(to->tss_sel, 0);
}