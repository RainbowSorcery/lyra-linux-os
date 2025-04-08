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

void list_head_insert(list_t *list, list_node_t *node)
{
    if (list->count == 0)
    {
        list->first = node;
    }
    else
    {
        // 修改头节点指向节点
        list_node_t *head = list->first;
        head->pre = node;
        node->next = head;
        list->first = node;
    }
    list->count++;
}

void list_last_insert(list_t *list, list_node_t *node)
{
    if (list->count == 0)
    {
        list->first = node;
        list->last = node;
    }
    else
    {
        list_node_t *lastNode = list->last;
        lastNode->next = node;
        node->pre = lastNode;
    }

    list->count++;
}

void list_remove(list_t *list, list_node_t *node)
{
    list->count--;

    // 如果删除的是头节点 那么设置头节点为下一个节点
    if (list->first == node) {
        list->first = list->first->next;
    }

    // 如果删除的是尾节点 那么将之前的尾节点的前驱节点设置成尾节点
    if (list->last == node) {
        list->last = list->last->pre;
    }

    // 如果删除的是中间节点 那么将删除节点的上一个节点连接删除节点的下一个节点
    if (node->pre != 0) {
        node->pre->next = node->next;
    }
    
    // 清空删除节点的指向关系
    node->pre = node->next = (list_node_t*)0;
}