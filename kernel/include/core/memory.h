#include "../../../common/types.h"
#include "../tools/bitmap.h"
#include "../ipc/mutex.h"
#include "../../../common/boot_info.h"

// 页起始地址
#define MEM_EXT_START (1024 * 1024)
// 页大小 4kb
#define MEM_PAGE_SIZE (4 * 1024)


typedef struct _addr_alloc_t
{
    // 互斥锁，避免在内存分配的时候出现冲突
    mutex_t lock;

    // 位图
    bit_map_t *bitmap;

    // 开始地址
    unint32_t start;

    // 内存空间大小
    unint32_t size;

    // 页大小
    unint32_t page_size;
} addr_alloc_t;
;
void memory_init(boot_info_t *boot_info);