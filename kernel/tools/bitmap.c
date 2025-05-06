#include "bitmap.h"
#include "../include/tools/klib.h"


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

    kernel_memset(bits, init_bit, count);
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
int bitmap_is_set(bit_map_t *bitmap, int index)
{
    return bitmap_get_bit(bitmap, index) ? 1 : 0;
}

// 寻找指定长度为指定值的位图，设置成另一个值
int bitmap_alloc_nbits(bit_map_t *bitmap, int bit, int count)
{
    int serach_index = 0;
    int ok_index = -1;
    while (serach_index < bitmap->bit_count)
    {
        // 判断当前字节是否为目标字节 如果不是则跳出循环遍历下一个位图
        if (bitmap_get_bit(bitmap, serach_index) != bit)
        {
            serach_index++;
            continue;
        }

        ok_index = serach_index;
        int i = 0;
        // 遍历后面几个连续的位图值是否为目标值，如果有一个不是则跳出循环，重新进行遍历
        for (i = 0; i < count && serach_index < bitmap->bit_count; i++)
        {
            if (bitmap_get_bit(bitmap, i) != bit)
            {
                ok_index = -1;
                break;
            }
        }
        // 如果遍历了整个长度都是目标值那么同意修改位图
        if (i >= count)
        {
            bitmap_set_bit(bitmap, serach_index, count, bit);
            return 0;
        }
    }

    return -1;
}