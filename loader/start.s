.code16
.text
.global _start
.extern loader_entry
_start:
    mov $0, %ebx
    mov $0x534d4150, %edx
    mov $0xe820, %eax
    mov $24, %ecx
    int $0x15
    call loader_entry