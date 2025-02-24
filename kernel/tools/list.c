#include "../include/tools/list.h"

void list_init(list_t *list)
{
    // 初始化头节点 尾节点
    list->first = list->last = (list_node_t *)0;
    list->count = 0;
}
