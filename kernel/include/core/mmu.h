#ifndef MMU_H
#define MMU_H

#include "../../../common/types.h"
#include "../../../common/cpu_instr.h"

// 页目录表项数
#define PDE_COUNT 1024

#define PTE_P 1

#define PDE_W (1 << 1)
#define PTE_W (1 << 1)
#define PDE_U (1 << 2)

/**
 * 页目录结构
 */
typedef union _pde_t
{
    unint32_t v;

    struct 
    {
        // 存在位 标示该页是否在物理内存中
        uint16_t present : 1;
        // 读写位 该页是否有读写权限
        uint16_t write_enable : 1;
        // 用户/超级用户位 用户或只有内核模式内访问该块
        uint16_t user_mode_acc : 1;
        // 写穿透位 控制CPU缓存的写策略
        uint16_t write_throuth : 1;
        // 缓存禁用位 是否禁用该页的CPU缓存
        uint16_t page_cache_disable : 1;
        // 访问位 记录该页是否被访问过
        uint16_t accessed : 1;
        // 忽略位 通常为 0，不起作用（除非开启了大页模式）。
        uint16_t ignore: 1;
        // 页大小 0: 4k 1: 4M
        uint16_t page_size : 1;
        // 无效位 Intel 手册规定，当 PDE 指向一个页表时，第 8 位是被硬件忽略的。
        uint16_t ignored : 1;
        // 可用位 留给操作系统使用。3位
        uint16_t avail : 3;
        // 物理地址
        unint32_t phy_pt_addr : 20;
    };

} pde_t;


/**
 * 页表结构
 */
typedef union _pte_t
{
    unint32_t v;

    struct 
    {
        // 存在位 标示该页是否在物理内存中
        uint16_t present : 1;
        // 读写位 该页是否有读写权限
        uint16_t write_enable : 1;
        // 用户/超级用户位 用户或只有内核模式内访问该块
        uint16_t user_mode_acc : 1;
        // 写穿透位 控制CPU缓存的写策略
        uint16_t write_throuth : 1;
        // 缓存禁用位 是否禁用该页的CPU缓存
        uint16_t page_cache_disable : 1;
        // 访问位 记录该页是否被访问过
        uint16_t accessed : 1;
        // 脏位，表示被写过
        uint16_t dirty : 1;
        // 页属性表位 与PWT和PCD联合使用，确定该页的内存属性
        uint16_t pat : 1;
        // 全局位 作用：当刷新 TLB (转换后备缓冲区) 时（例如切换进程 CR3 寄存器变化时），标记为 Global 的条目不会被清除。常用于内核部分，因为所有进程共享内核空间，不需要刷新。
        uint16_t global : 1;
        // 可用位 留给操作系统使用。3位
        uint16_t avail : 3;
        // 物理地址
        unint32_t phy_pt_addr : 20;
    };

} pte_t;

/**
 * 设置页目录
 */
static inline void mmu_set_page_dir(unint32_t addr) 
{
    write_cr3(addr);
}


/**
 * 根据线性地址的规则取前10位就是页目录的索引
 */
static inline unint32_t ped_index(unint32_t v_start) 
{
    return v_start >> 22;
}


/**
 * 根据线性地址的规则取中间10位就是页表的索引
 */

 static inline unint32_t pte_index(unint32_t v_start) 
 {
     return (v_start >> 12) & 0x3ff;
 }


 /**
  * 获取页目录对应的页表地址 20位中的高12位存储的物理地址
  */
static inline unint32_t pde_addr(pde_t *pde) 
{
    return pde->phy_pt_addr << 12;
}

#endif