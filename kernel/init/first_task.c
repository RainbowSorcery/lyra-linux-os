#include "../include/core/task.h"
#include "../tools/log.h"

int first_task_main(void)
{
    int count = 0;

    for (;;)
    {
        count++;
        log_printf("first task count:%d", count);
    }

    return 0;
};