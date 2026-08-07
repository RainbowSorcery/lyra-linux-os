    .code32
    .text
    .global first_task_entry
    .extern first_task_main
first_task_entry:
    mov $16, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    jmp first_task_main