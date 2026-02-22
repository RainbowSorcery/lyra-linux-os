#include "../include/core/memory.h"
#include "../kernel/tools/log.h"
#include "../include/tools/klib.h"
#include "../include/core/mmu.h"

static addr_alloc_t paddr_alloc;

// 页目录列表
static pde_t kernel_page_dir[PDE_COUNT] __attribute__((aligned(MEM_PAGE_SIZE)));

static void addr_alloc_init(addr_alloc_t *addr_alloc, unit8_t *bits, unint32_t start, unint32_t size, unint32_t page_size) 
{
    mutex_init(&addr_alloc->lock);
    addr_alloc->page_size = page_size;
    addr_alloc->size = size;
    addr_alloc->start = start;
    // 位图数 / 页大小计算出可以创建多少页
    bitmap_init((bit_map_t*)&addr_alloc->bitmap, bits, addr_alloc->size / page_size, 0);
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

unint32_t total_mem_size(boot_info_t *boot_info)
{
    unint32_t size = 0;
    for (int i = 0; i < boot_info->ram_region_count; i++)
    {
        size += boot_info->ram_regin_confg[i].size;
    }
    return size;
}

void show_memory_info(boot_info_t *boot_info) 
{
    log_printf("mem region:");

    for (int i = 0; i < boot_info->ram_region_count; i++)
    {
        log_printf("[%d]: 0x%x-0x%x", i, 
            boot_info->ram_regin_confg[i].start, 
            boot_info->ram_regin_confg[i].size);
    }
}


int memory_create_map(pte_t *page_dir, unint32_t v_start, unint32_t p_addr, unint32_t page_count, unint32_t perm)
{
    for (int i = 0; i 《 page_count; i++)
    {
        pte_t *pre = find_pte();
    }
}

void create_kernel_page()
{
    extern unit8_t s_text[], e_text[], s_data[], kener_base[], e_data[];
    static memory_map_t kernel_map[] =
        {
            {kener_base, s_text, kener_base, 0},
            {s_text, e_text, s_text, 0},
            {s_data, e_data, s_data, 0}
        };

    // 初始化页表与线性地址与物理地址之间的映射
    for (int i = 0; i < (sizeof(kernel_map) / sizeof(memory_map_t)); i++) 
    {
        memory_map_t *map = &kernel_map[i];

        unint32_t start = down2((unint32_t)map->v_start, MEM_PAGE_SIZE);
        unint32_t end = up2((unint32_t)map->v_end, MEM_PAGE_SIZE);

        int page_count = (end - start) / MEM_PAGE_SIZE;

        // 建立虚拟内存与物理内存的映射关系
        memory_create_map(kernel_page_dir, start, (unint32_t)map->p_start, page_count, map->perm);
    }
}

void memory_init(boot_info_t *boot_info)
{
    extern unit8_t *mem_free_start;

    log_printf("mem init");
    show_memory_info(boot_info);

    // 1M以上内存空间
    unint32_t mem_up1MB_free = total_mem_size(boot_info) - MEM_EXT_START;

    mem_up1MB_free = down2(mem_up1MB_free, MEM_PAGE_SIZE);
    log_printf("free memory: 0x%x, size:0x%x", MEM_EXT_START, mem_up1MB_free);
        
    unit8_t *mem_free = (unit8_t *)&mem_free_start;

    addr_alloc_init(&paddr_alloc, mem_free, MEM_EXT_START, mem_up1MB_free, mem_up1MB_free / MEM_PAGE_SIZE);

    // 跳过位图缓存区内存位置
    mem_free += bitmap_byte_count(paddr_alloc.size / MEM_PAGE_SIZE);
    
    // 创建内核页表
    create_kernel_page();

    // 设置页目录
    mmu_set_page_dir((unint32_t)kernel_page_dir);
}
