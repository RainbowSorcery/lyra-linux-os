SECTIONS {
    . = 0x100000; /*设置段起始地址*/
    .text : {
        *(.text)
    }
    
    .rodata : {
        *(.rodata)
    }
    
    .data : {
        *(.data)
    }
    .bss : {
        *(.bss)
    }

}