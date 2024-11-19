#include "types.h"

#define BOOT_RAM_REGION_MAX 10

// 内存信息结构体
typedef struct boot_info_t
{
    struct
    {
        unint32_t start;
        unint32_t size;
    } ram_regin_confg[BOOT_RAM_REGION_MAX];

    int ram_region_count;
} boot_info_t;
