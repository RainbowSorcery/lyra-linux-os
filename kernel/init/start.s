#include "../include/cpu.h"

.text
.global _start
.extern kernel_init
.extern init_main
_start:
    // 模拟函数调用, 先将loader传入的值获取到保存到eax中, 再将eax压到栈中便于kernel_init函数获取到
    // 具体原理查看gcc c语言汇编调用原理
    push %ebp
    mov %ebp, %esp
    mov -0x18(%esp), %eax

    // 压栈 由kernel_init出栈获取到参数
    push %eax
    call kernel_init

gdt_reload:
    mov $8, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov $(stack  + 8*1024), %esp
    jmp init_main

    .bss
.comm stack, 8*1024