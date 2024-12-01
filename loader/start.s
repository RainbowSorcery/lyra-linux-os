.code16
.text
.global _start
.extern loader_entry
_start:
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