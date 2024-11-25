// 生成十六位机器码
__asm__(".code16gcc");

#include "boot.h"

#define LOADER_START_ADDR 0x8000 // loader加载的地址

void boot_entry(void)
{
    // 利用函数指针调用0x8000地址的函数，
    ((void (*)(void))LOADER_START_ADDR)();
}
