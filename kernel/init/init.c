#include "init.h"
#include "../common/boot_info.h"

void kernel_init(boot_info_t *boot_info)
{
    
    int *p = (int *)0xa0000;

    *p = 0xf;
    for (;;)
    {
    }
}