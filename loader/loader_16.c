#include "../common/boot_info.h"
#include "../common/types.h"
#include "../common/cpu_instr.h"
#include "loader_16.h"

static boot_info_t boot_info;

__asm__(".code16gcc");

static void show_msg(const char *msg)
{

    char printChar;
    while (*msg != '\0')
    {
        printChar = *msg;
        asm("mov $0x0e, %%ah\n\t"
            "mov %[ch], %%al\n\t"
            "int $0x10"
            :
            : [ch] "r"(printChar)
            : "ah", "al");

        msg++;
    }
}

uint16_t gdt_table[][4] = {
    {0, 0, 0, 0},
    {0xffff, 0x0000, 0x9a00, 0x00cf},
    {0xffff, 0x0000, 0x9200, 0x00cf}};

// 保护模式设置
static void enter_protect_mode(void)
{
    cli();
    // 开启A2地址线
    unit8_t value = inb((uint16_t)0x92);
    outb(0x92, (value | 0x2));
    lgdt((unint32_t)gdt_table, (uint16_t)sizeof(gdt_table));
}

// 读取内存信息
static void detect_memory(void)
{
    e820_entry entry;

    show_msg("try to detect memory:");
    boot_info.ram_region_count = 0;

    int contId = 0;
    int signature = 0;
    int bytes = 0;

    for (int i = 0; i < BOOT_RAM_REGION_MAX; i++)
    {
        e820_entry *point_entry = &entry;
        asm("int $0x15" : "=a"(signature), "=b"(contId), "=c"(bytes)
            : "a"(0xe820), "c"(24), "b"(contId), "d"(0x534d4150), "D"(point_entry)
            : "memory");

        if (signature != 0x534D4150)
        {
            show_msg("signature error");
            break;
        }

        if (bytes > 20 && (point_entry->acpi & 0x0001) == 0)
        {
            continue;
        }

        // 保存内存信息到结构体中
        if (point_entry->type == 1)
        {
            boot_info.ram_regin_confg[boot_info.ram_region_count].start = entry.base_addr_l;
            boot_info.ram_regin_confg[boot_info.ram_region_count].size = entry.length_l;
            boot_info.ram_region_count++;
        }

        // contId = 0表示内存读取完毕
        if (contId == 0)
        {
            break;
        }
    }

    show_msg("memeory read done\n");
}

void loader_entry()
{
    detect_memory();

    enter_protect_mode();

    for (;;)
    {
    }
}
