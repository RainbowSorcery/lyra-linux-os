SECTIONS {
    PROVIDE(kener_base = .);
    . = 0x100000; /*设置段起始地址*/

    PROVIDE(s_text = .);
    .text : {
        *(EXCLUDE_FILE(*first_task*) .text)
    }
    
    .rodata : {
        *(EXCLUDE_FILE(*first_task*) .rodata)
    }
    
    PROVIDE(e_text = .);
    . = ALIGN(4096);

    PROVIDE(s_data = .);
    .data : {
        *(EXCLUDE_FILE(*first_task*) .data)
    }
    PROVIDE(e_data = .);
    PROVIDE(s_bss = .);

    .bss : {
        *(EXCLUDE_FILE(*first_task*) .bss)
    }
    
    PROVIDE(e_bss = .);
    e_data = .;

    /* First task section 0x8000000之前的地址操作系统用，之后的地址用户进程使用 */
    . = 0x8000000;
    PROVIDE(s_first_task = LOADED(.first_task));
    .first_task : AT(e_data) {
        *first_task_entry*(.text .rodata .bss .data)
        *first_task*(.text .rodata .bss .data)
    }

    e_first_task  = LOADADDR(.first_task) + SIZEOF(.first_task);

    PROVIDE(mem_free_start = e_first_task);
}