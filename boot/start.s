    #include "boot.h"
    .code16
    .text
    .global _start
    // 标识.extern才能调用c语言函数
    .extern boot_entry

_start:
    // 初始化寄存器配置 将寄存器设置为0,并将栈底指针设置为0x7c00, 0x500-0x7c00表示我们的栈存储区域, 0x400-0x4ff是bios数据存储区
    mov $0, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov $0x7c00, %esp

    // 读取loader分区信息
    mov $0x8000, %bx
    mov $0x2, %ah
    mov $0x2, %cx
    mov $64, %al
    mov $0x0080, %dx
    int $0x13

    jmp boot_entry

.section boot_end, "ax"
boot_sig: .byte 0x55, 0xaa