SECTIONS {
    PROVIDE(kener_base = .);
    . = 0x100000; /*设置段起始地址*/

    PROVIDE(s_text = .);
    .text : {
        *(.text)
    }
    
    .rodata : {
        *(.rodata)
    }
    
    PROVIDE(e_text = .);
    PROVIDE(s_data = .);
    .data : {
        *(.data)
    }
    PROVIDE(e_data = .);
    .bss : {
        *(.bss)
    }
    PROVIDE(mem_free_start = .);
}