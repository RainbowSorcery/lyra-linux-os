#ifndef SAMAPHORE_H
#define SAMAPHORE_H

#include "../tools/list.h"


typedef struct _semaphore
{
    // 资源数
    int count;
    // 等待队列
    list_t wait_list;
} semaphore_t;

void semaphore_init(semaphore_t *sem, int count);

// 信号量 等待
void semaphore_wait(semaphore_t *sem);
// 信号量 发
void semaphore_notify(semaphore_t *sem);


#endif