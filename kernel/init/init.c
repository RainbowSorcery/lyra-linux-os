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
        sys_sleep(5000);
    }
}

void init_main()
{

    init_task_managment();
    int count = 0;

    task_first_init();
    task_init(&init_task, (unint32_t)init_task_entry, (unint32_t)&init_task_stack[1024], "init_task");

    for (;;)
    {
        count++;
        log_printf("init main count:%d", count);

        sys_sleep(5000);
    }
}
