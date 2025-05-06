#include "bitmap.h"
#include "klib.h"


int bitmap_byte_count(int bit_count)
{
    int bytes = bit_count / 8;
    if (bit_count % 8 != 0)
    {
        bytes++;
    }
    return bytes;
}

void bitmap_init(bit_map_t *bitmap, unit8_t *bits, int count, int init_bit)
{
    bitmap->bit_count = count;
    bitmap->bits = bits;

    int bytes = bitmap_byte_count(count);

    kernel_memset(bits, init_bit, value)
}


// 获取指定位图数据
int bitmap_get_bit(bit_map_t *bitmap, unit8_t index)
{
    return bitmap->bits[index / 8] >> (index % 8) & 0x01;
}

// 将连续指定位图设置成指定值
void bitmap_set_bit(bit_map_t *bitmap, int index, int count, int bit)
{
    for (int i = 0; i < count && (index < bitmap->bit_count); i++)
    {
        if (bit)
        {
            bitmap->bits[index / 8] |= (1 >> (index % 8));
        }
        else 
        {
            bitmap->bits[index / 8] &= ~(1 >> (index % 8));
        }
    }
}

// 判断指定位图释放被使用
int bitmap_set_bit(bit_map_t *bitmap, int index)
{
    return bitmap_get_bit(bit_map_t, index) ? 1 : 0;
}
// 连续分配指定长度位图
int bitmap_alloc_nbits(bit_map_t *bitmap, int bit, int count)
{

}