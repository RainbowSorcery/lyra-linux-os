#include "../../common/types.h"

typedef struct _bitmap_t
{
    // 页表位数 也就是页表个数
    int bit_count;
    // 页表数据指针
    unit8_t *bits;
} bit_map_t;

// 获取位图占用字节数
void bitmap_byte_count(int bit_count) 

// 初始化页表
void bitmap_init(bit_map_t *bitmap, unit8_t *bits, int count, int init_bit);

// 获取位图长度
int bitmap_byte_count(int bit_count);

// 获取指定位图数据
int bitmap_get_bit(bit_map_t *bitmap, unit8_t index);

// 将连续指定位图设置成指定值
void bitmap_set_bit(bit_map_t *bitmap, int index, int count, int bit);

// 判断指定位图释放被使用
int bitmap_set_bit(bit_map_t *bitmap, int index);

// 连续分配指定长度位图
int bitmap_alloc_nbits(bit_map_t *bitmap, int bit, int count);