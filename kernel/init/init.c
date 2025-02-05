#include "init.h"
#include "../common/boot_info.h"
#include "../include/cpu.h"

void kernel_init(boot_info_t *boot_info)
{
    cpu_init();
}

void init_main()
{
    // int a = 10 / 0;

    for (;;)
    {
    }
}