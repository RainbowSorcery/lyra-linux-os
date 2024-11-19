typedef struct e820_entry
{
    unint32_t base_addr_l; // 基地址，表示该内存区域的起始地址
    unint32_t base_addr_h; // 基地址，表示该内存区域的起始地址
    unint32_t length_l;    // 长度，表示该内存区域的大小
    unint32_t length_h;    // 长度，表示该内存区域的大小
    unint32_t type;        // 类型，表示该内存区域的用途或类型
    unint32_t acpi;        // ACPI扩展字段（可选）
} e820_entry;