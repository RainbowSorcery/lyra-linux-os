#include "../common/types.h"

// 进制在结构体中填充东西
#pragma pack(1)

typedef struct _segment_desc_t
{
    uint16_t limit_0_15;
    uint16_t base_0_15;
    unit8_t base_16_23;
    uint16_t attr;
    unit8_t base_24_31;
} segment_desc_t;

#pragma pack()