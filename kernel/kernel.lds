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
    . = ALIGN(4096);

    PROVIDE(s_data = .);
    .data : {
        *(.data)
    }
    PROVIDE(e_data = .);
    PROVIDE(s_bss = .);
    .bss : {
        *(.bss)
    }
    PROVIDE(e_bss = .);
    PROVIDE(mem_free_start = .);
}