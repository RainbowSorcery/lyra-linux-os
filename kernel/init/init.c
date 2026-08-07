#include "init.h"
#include "../common/boot_info.h"
#include "../include/cpu.h"
#include "../include/tools/list.h"
#include "../tools/log.h"
#include "../include/core/task.h"
#include "../../common/cpu_instr.h"
#include "../include/ipc/semaphore.h"
#include "../include/core/memory.h"

semaphore_t sem;

void kernel_init(boot_info_t *boot_info)
{
    cpu_init();
    memory_init(boot_info);

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

static unint32_t first_task_stack[1024];
static task_t init_task;


void move_to_first_task()
{
    task_t *first_task = task_current();
    log_printf("move to first task, entry=0x%x", first_task->tss.eip);

    __asm__ volatile (
        "movl %0, %%esp\n"
        "jmp *%1\n"
        :
        : "r"(first_task_stack + 1024), "r"(first_task->tss.eip)
    );
}

void init_main()
{

    init_task_managment();
    int count = 0;

    task_first_init();
    move_to_first_task();
}
