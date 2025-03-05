#include "../include/tools/list.h"

void list_init(list_t *list)
{
    // 初始化头节点 尾节点
    list->first = list->last = (list_node_t *)0;
    list->count = 0;
}

int list_is_empty(list_t *list)
{
    return list->count == 0;
}

int list_count(list_t *list)
{
    return list->count;
}

list_node_t *list_first(list_t *list)
{
    return list->first;
}

list_node_t *list_last(list_t *list)
{
    return list->last;
}
