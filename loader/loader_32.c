#include "types.h"
#include "cpu_instr.h"
#include "loader.h"

/*
 LBA48，最高可以读取48位256TB，读取磁盘
*/
static void read_disk(unint32_t sector, uint16_t selctor_count, unit8_t *buffer)
{
    // 设置硬盘读取位置 具体查看0x1f6寄存器位配置表
    outb(0x1f6, 0xe0);
    // 设置读取扇区数量高8字节
    outb(0x1f2, selctor_count >> 8);
    /*
        LBA48扇区读写位
        LBA4  24-31
        LBA5  32 - 39
        LBA6 40 - 47
        LBA1 0-7
        LBA2 8-15
        LBA3 16-23
    */
    unit8_t lba4 = (sector >> 24) & 0xff;
    // LBA48支持48位长度的扇区读，我们函数的传参只有32位，lba5和lba6没用到，所以直接设置为0。
    unit8_t lba5 = 0;
    unit8_t lba6 = 0;
    ;
    unit8_t lba1 = sector & 0xff;
    unit8_t lba2 = (sector >> 8) & 0xff;
    unit8_t lba3 = (sector >> 16) & 0xff;

    outb(0x1f3, lba4);
    outb(0x1f4, lba5);
    outb(0x1f5, lba6);

    // 读取扇区低8位
    outb(0x1f2, selctor_count & 0xff);
    outb(0x1f3, lba1);
    outb(0x1f4, lba2);
    outb(0x1f5, lba3);

    outb(0x1f7, 0x24);

    uint16_t *data_buffer = (uint16_t *)buffer;
    while (selctor_count--)
    {
        // 判断硬盘是否在忙，如果在忙则阻塞等待
        while ((inb(0x1f7) & 0x88) != 0x8)
        {
        }

        for (int i = 0; i < SECTOR_SIZE / 2; i++)
        {
            *data_buffer++ = inw(0x1f0);
        }
    }
}

#define CR4_PSE 1 << 4
// 开启分页
#define CR0_PG  1 << 31
// 页目录有效
#define PDE_P 1 << 0
// 页目录可写
#define PDE_W 1 << 1
// 页大小  4MB  
#define PDE_PS 1 << 7

void enable_page_mode()
{
    static unint32_t page_dir[1024] __attribute__((aligned(4096)));
    page_dir[0] = PDE_P | PDE_W | PDE_PS | 0;

    unint32_t cr4 = read_cr4();
    cr4 = cr4 | (CR4_PSE);
    write_cr4(cr4);

    write_cr3((unint32_t)page_dir);

    unint32_t cr0 = read_cr0();
    write_cr0(cr0 | CR0_PG);
}

void loader_kernel()
{
    read_disk(100, 500, (unit8_t *)SYSTEM_KERNEL_ADDRESS);

    // enable_page_mode();

    ((void (*)(boot_info_t *))0x100000)(&boot_info);

    for (;;)
    {
    }
}
