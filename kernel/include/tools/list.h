#include "../../../common/types.h"

#ifndef LIST_H
#define LIST_H

/**
 * 链表结点类型
 */
typedef struct _list_node_t
{
    struct _list_node_t *pre;  // 链表的前一结点
    struct _list_node_t *next; // 后继结点
} list_node_t;

/**
 * 带头结点和尾结点的单链表
 * 每个结点只需要一个指针，用于减少内存使用量
 */
typedef struct _list_t
{
    list_node_t *first; // 首结点
    list_node_t *last;  // 尾结点
    int count;          // 结点数量
} list_t;

void list_init(list_t *list);

int list_is_empty(list_t *list);

list_node_t *list_first(list_t *list);

list_node_t *list_last(list_t *list);

int list_count(list_t *list);

// 将节点插入头部
void list_head_insert(list_t *list, list_node_t *node);

// 将节点插入到尾部
void list_last_insert(list_t *list, list_node_t *node);

// 计算指定节点偏移位置
#define offset_in_parent(parent_type, node_name) \
    (unint32_t) & (((parent_type *)0)->node_name)

// 节点所在位置减去偏移位置等于当前结构体开始地址
#define parent_addr(node, parent_type, node_name) \
    ((unint32_t)node) - offset_in_parent(parent_type, node_name)

// 将地址转换为结构体
#define list_node_parent(node, parent_type, node_name) \
    (parent_type *)(node ? parent_addr(node, parent_type, node_name) : 0)

#endif