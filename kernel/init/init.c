#include "init.h"
#include "../common/boot_info.h"
#include "../include/cpu.h"
#include "../include/tools/list.h"
#include "../tools/log.h"

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

void init_task_entry() 
{
    int count = 0;

    for (;;)
    {
        count++;
        log_printf("init task count:%d", count);
    }
}

void init_main()
{
    int count = 0;

    for (;;)
    {
        count++;
        log_printf("init main count:%d", count);
    }
}