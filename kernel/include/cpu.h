#include "../common/types.h"

// 禁止在结构体中填充东西
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

#define SEG_G (1 << 15)
#define SEG_D (1 << 14)
#define SEG_P (1 << 7)

#define SEG_DPL0 (0 << 5)
#define SEG_DPL3 (3 << 5)

#define SEG_S_SYSTEM (0 << 4)
#define SEG_S_NORMAL (1 << 4)

#define SEG_TYPE_CODE (1 << 3)
#define SEG_TYPE_DATA (0 << 3)

#define SEG_TYPE_RW (1 << 1)