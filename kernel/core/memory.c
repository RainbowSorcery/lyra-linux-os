#include "../include/core/memory.h"
#include "../kernel/tools/log.h"

static void addr_alloc_init(addr_alloc_t *addr_alloc, unit8_t *bits, unint32_t start, unint32_t size, unint32_t page_size) 
{
    mutex_init(&addr_alloc->lock);
    addr_alloc->page_size = page_size;
    addr_alloc->size = size;
    addr_alloc->start = start;
    // 位图数 / 页大小计算出可以创建多少页
    bitmap_init(addr_alloc->bitmap, bits, addr_alloc->size / page_size, 0);
}

// 内存分配
static unint32_t addr_alloc_page(addr_alloc_t *alloc, int page_count)
{
    mutex_lock(&alloc->lock);

    int start_index =  bitmap_alloc_nbits(alloc->bitmap, 0, page_count);
    if (start_index >= 0)
    {
        // 返回开始地址 page大小 * 开始索引 + 偏移计算得出
        return start_index * alloc->page_size + alloc->start;
    }

    mutex_unlock(&alloc->lock);

    return -1;
}


static void addr_alloc_free(addr_alloc_t *alloc, int addr, int page_count)
{

    mutex_lock(&alloc->lock);
    int pg_index = (addr - alloc->start) / alloc->page_size;
    bitmap_set_bit(alloc->bitmap, pg_index, page_count, 0);

    mutex_unlock(&alloc->lock);

}

void memory_init(boot_info_t *boot_info)
{
    addr_alloc_t addr_alloc;
    unit8_t bits[8];

    addr_alloc_init(&addr_alloc, bits, 0x1000, 64 * 4096, 4096);

    for (int i = 0; i < 32; i++)
    {
        unint32_t addr = addr_alloc_page(&addr_alloc, 2);
        log_printf("addr:0x%x", addr);
    }
    int baseAddr = 0x1000;
    for (int i = 0; i < 32; i++)
    {
        addr_alloc_free(&addr_alloc, baseAddr, 2);
        baseAddr += 8192;
    }

    log_printf("");
}