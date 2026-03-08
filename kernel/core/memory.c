#include "../include/core/memory.h"
#include "../kernel/tools/log.h"
#include "../include/tools/klib.h"
#include "../include/core/mmu.h"

// 位图对象 用于分配内存
static addr_alloc_t paddr_alloc;

// 页目录列表
static pde_t kernel_page_dir[PDE_COUNT] __attribute__((aligned(MEM_PAGE_SIZE)));

static void addr_alloc_init(addr_alloc_t *addr_alloc, unit8_t *bits, unint32_t start, unint32_t size, unint32_t page_size) 
{
    mutex_init(&addr_alloc->lock);
    addr_alloc->page_size = page_size;
    addr_alloc->size = size;

    int bytes = bitmap_byte_count(addr_alloc->size / page_size);
    addr_alloc->start = up2(bits + bytes, MEM_PAGE_SIZE);
    // 位图数 / 页大小计算出可以创建多少页
    bitmap_init(&addr_alloc->bitmap, bits, addr_alloc->size / page_size, 0);
}

// 内存分配
static unint32_t addr_alloc_page(addr_alloc_t *alloc, int page_count)
{
    // mutex_lock(&alloc->lock);

    int start_index =  bitmap_alloc_nbits(&alloc->bitmap, 0, page_count);
    unint32_t result = -1;
    if (start_index >= 0)
    {
        // 返回开始地址 page大小 * 开始索引 + 偏移计算得出
        result = start_index * alloc->page_size + alloc->start;
    }

    // mutex_unlock(&alloc->lock);

    return result;
}


static void addr_alloc_free(addr_alloc_t *alloc, int addr, int page_count)
{

    mutex_lock(&alloc->lock);
    int pg_index = (addr - alloc->start) / alloc->page_size;
    bitmap_set_bit(&alloc->bitmap, pg_index, page_count, 0);

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

pte_t *find_pte(pde_t *page_dir, unint32_t v_addr, unit8_t alloc) 
{
    // 根据索引下标寻找指定页目录
    pde_t *pde =  page_dir + ped_index(v_addr);

    // 页表项基地址
    pte_t *base_page_table;

    if (pde->present) {
        base_page_table =(pte_t*) pde_addr(pde);
    }
    else
    {
        // 分配页数据
        if (alloc == 0)
        {
            return (pte_t *)0;
        }

        unint32_t addr = addr_alloc_page(&paddr_alloc, 1);
        if (addr == 0 || addr == -1)
        {
            return (pte_t *)0;
        }

        base_page_table = (pte_t *) addr;

        // 重新初始化内存数据，避免有错误信息
        kernel_memset(base_page_table, 0, MEM_PAGE_SIZE);
        pde->v = addr | PTE_P | PDE_U | PDE_W;
    }

    return base_page_table + pte_index(v_addr);
}

int memory_create_map(pde_t *page_dir, unint32_t v_addr, unint32_t p_addr, unint32_t page_count, unint32_t perm)
{
    for (int i = 0; i < page_count; i++)
    {
        log_printf("create map v-0x%x, p-0x%x, perm:0x%x", v_addr, p_addr, perm);
        // 查找页表项 如果找不到则新建页表想
        pte_t *pte = find_pte(page_dir, v_addr, 1);

        if (pte == 0)
        {
            log_printf("create pte failed. pte=0");
            return -1;
        }


        log_printf("pte addr:0x%x", (unint32_t)pte);
        pte->v = p_addr | perm | PTE_P;

        v_addr += MEM_PAGE_SIZE;
        p_addr += MEM_PAGE_SIZE;
    }
     return 0;  
}

/**
 * 创建内存虚拟内存页
 */
void create_kernel_page()
{
    extern unit8_t s_text[], e_text[], s_data[], kener_base[], e_data[], s_bss[], e_bss[];
    static memory_map_t kernel_map[] =
        {
            {(void*)0x0, (void*)0x1000, (void*)0x0, PTE_W},
            {kener_base, s_text, kener_base, PTE_W},
            {s_text, e_text, s_text, 0},
            {s_data, (void*) MEM_EBDA_START, s_data, PTE_W}
        };

    // 初始化页表与线性地址与物理地址之间的映射
    for (int i = 0; i < (sizeof(kernel_map) / sizeof(memory_map_t)); i++) 
    {
        memory_map_t *map = &kernel_map[i];

        unint32_t start = down2((unint32_t)map->v_start, MEM_PAGE_SIZE);
        unint32_t end = up2((unint32_t)map->v_end, MEM_PAGE_SIZE);
        unint32_t paddr = down2((unint32_t)map->p_start, MEM_PAGE_SIZE);


        int page_count = (end - start) / MEM_PAGE_SIZE;

        // 建立虚拟内存与物理内存的映射关系
        memory_create_map(kernel_page_dir, start, (unint32_t)paddr, page_count, map->perm);
    }
}

unint32_t memory_create_user_virtual_memory() 
{
    // 1. 获取一页数据用于创建页表项
    pte_t* page_dir = (pte_t *)addr_alloc_page(&paddr_alloc, 1);
    if (page_dir == 0) {
        return 0;
    }
    kernel_memset((void *)page_dir, 0, MEM_PAGE_SIZE);


    // 0x80000000以上空间作为进程内存 以下作为与操作系统共享的内存，方便调用操作系统API
    unint32_t index = ped_index(MEM_TASK_BASE);

    log_printf("[MEM] user_pde_start = %d (0x%08x)\n", index, MEM_TASK_BASE);
    

    for (int i = 0; i < index; i++)
    {
        page_dir[i].v = kernel_page_dir[i].v;
    }

    log_printf("[MEM] Copied %d page directory entries\n", index);
    return (unint32_t)page_dir;
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

    addr_alloc_init(&paddr_alloc, mem_free, MEM_EXT_START, mem_up1MB_free, down2(mem_up1MB_free / MEM_PAGE_SIZE, MEM_PAGE_SIZE));

    // 跳过位图缓存区内存位置
    mem_free += bitmap_byte_count(paddr_alloc.size / MEM_PAGE_SIZE);
    
    // 创建内核页表
    create_kernel_page();

    // 设置页目录
    mmu_set_page_dir((unint32_t)kernel_page_dir);
}
