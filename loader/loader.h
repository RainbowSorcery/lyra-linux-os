#include "../common/boot_info.h"

// 扇区大小
#define SECTOR_SIZE 512

// 内核程序地址
#define SYSTEM_KERNEL_ADDRESS (1024 * 1024)

void protected_mode_entry();

extern boot_info_t boot_info;
