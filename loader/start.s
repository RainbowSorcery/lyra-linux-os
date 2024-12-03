.code16
.text
.global _start
.extern loader_entry
_start:

    // 设置VGA显卡配置 16色模式
    mov $0x03, %al 
    mov $0x00, %ah
    int $0x10

    jmp loader_entry

    .code32
    .text
    .global protected_mode_entry
    .extern loader_kernel
protected_mode_entry:
    mov $16, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    ljmpl $8, $loader_kernel