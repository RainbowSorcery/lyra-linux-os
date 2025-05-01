#ifndef MUTEX_H
#define MUTEX_H

#include "../core/task.h"

typedef struct _mutex_t 
{
    // 由于会遇到进程中会嵌套加锁，所以需要添加一个成员遍历来标识加锁数
    int lock_count;
    // 当前占用锁的进程
    task_t *owner;
    // 等待使用锁的任务队列
    list_t *wait_list;
} mutex_t;

// 初始化互斥锁
void mutex_init(mutex_t *lock);

// 上锁
void mutex_lock(mutex_t *lock);

// 解锁
void mutex_unlock(mutex_t *lock);

#endif