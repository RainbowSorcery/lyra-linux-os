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
    jmp $8, $gdt_reload

gdt_reload:
    mov $16, %ax
    mov %ax, %ds
    mov %ax, %ss
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    mov $(stack  + 8*1024), %esp
    jmp init_main

    .bss
.comm stack, 8*1024
    
.macro exception_handler name num with_error_code
    .text
    .extern do_handler_\name
    .global exception_handler_\name
    exception_handler_\name:
        .if \with_error_code == 0
            push $0
        .endif
        push $\num
        pusha
        push %ds
        push %es
        push %fs
        push %gs

        push %esp
        call do_handler_\name
        add $(1*4), %esp

        pop %gs
        pop %fs
        pop %es
        pop %ds
        popa
        // 将\num和error_code弹栈
        add $(2*4), %esp
        iret
.endm
// 内置异常
exception_handler unknown -1, 0
exception_handler divide 0, 0
exception_handler debug 1, 0
exception_handler nmi 2, 0
exception_handler breakpoint 3, 0
exception_handler overflow 4, 0
exception_handler bound_range 5, 0
exception_handler invalid_opcode 6, 0
exception_handler device_not_available 7, 0
exception_handler double_fault 8, 1
exception_handler coprocessor_segment_overrun 9, 0
exception_handler invalid_tss 10, 1
exception_handler segment_not_present 11, 1
exception_handler stack_fault 12, 1
exception_handler general_protection 13, 1
exception_handler page_fault 14, 1
exception_handler fpu_error 16, 0
exception_handler alignment_check 17, 1
exception_handler machine_check 18, 0
exception_handler simd_floating_point 19, 0
exception_handler virtualization 20, 0
exception_handler control_protection 21, 1

// 8259a芯片中断
// 时钟中断
exception_handler time 0x20, 0
// 键盘中断
exception_handler keyboard 0x21, 0
// 鼠标中断
exception_handler mouse 0x2c, 0