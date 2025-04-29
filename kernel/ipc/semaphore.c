#include "../include/ipc/semaphore.h"
#include "../include/tools/list.h"
#include "../include/core/task.h"

void semaphore_init(semaphore_t *sem, int count)
{
    sem->count = count;
    list_init(&sem->wait_list);
}


void semaphore_wait(semaphore_t *sem)
{
    if (sem->count == 0)
    {
        task_t *current_task = task_current();
        task_set_block(current_task);
        list_last_insert(&sem->wait_list, &current_task->wait_node);
        task_dispach();
    }
    else
    {
        sem->count--;
    }
}

void semaphore_notify(semaphore_t *sem)
{
    // 判断是否有资源 如果没有资源那么将当前进程加入到等待队列中直到有资源运行，
    // 如果有资源则资源-1，进程继续运行 运行完成后资源+1
    if (list_count(&sem->wait_list) > 0) 
    {
        list_node_t *wait_node = list_last(&sem->wait_list);
        task_t *run_task = list_node_parent(wait_node, task_t, run_node);
        task_set_ready(run_task);
        task_dispach();
    }
    else
    {
        sem->count++;
    }
}