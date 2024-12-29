#include <common/types.h>

#pragma pack(1)

typedef struct _segment_desc_t
{
    uint16_t limit15_0;
    uint16_t base_15_0;
    unit8_t base_23_16;
    uint16_t attr;
    unit8_t base_31_24;
} _segment_desc_t;

#pragma pack()