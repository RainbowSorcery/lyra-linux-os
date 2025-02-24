#include "../../../common/types.h"

#ifndef LIST_H
#define LIST_H

/**
 * 链表结点类型
 */
typedef struct _list_t
{
    list_node_t *first; // 头结点
    list_node_t *last;  // 尾结点
    int count;          // 结点数量
} list_t;

void list_init(list_t *list);

#endif