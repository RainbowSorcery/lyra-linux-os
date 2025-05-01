#include "../include/ipc/mutex.h"
#include "../include/irq.h"


// 初始化互斥锁
void mutex_init(mutex_t *lock)
{
    lock->lock_count = 0;
    list_init(lock->wait_list);
    lock->owner = (task_t *)0;
}

// 上锁
void mutex_lock(mutex_t *lock)
{
    irq_state_t state = irq_enter_protection();
    task_t *current_task = task_current();
    // 判断是否有进程占用锁 如果没有进程占用的话那么直接加锁
    if (lock->owner == 0) 
    {
        lock->owner = current_task;
        lock->lock_count = ++lock->lock_count;
    }
    else if (lock->owner == current_task)
    {
        // 如果当前进程中嵌套锁了 那么锁计数+1
        lock->lock_count = lock->lock_count++;
    }
    else if (lock->owner != current_task)
    {
        // 如果想要加锁的进程不等于已经加锁的进程，那么把该进程放到等待队列中 并将进程从就绪队列中移除
        task_set_block(current_task);
        list_last_insert(lock->wait_list, &current_task->wait_node);
        task_dispach();
    }
    irq_leave_protection(state);
}

// 解锁
void mutex_unlock(mutex_t *lock)
{
    irq_state_t state = irq_enter_protection();

    // 在解锁的时候先判需要判断是否是当前进程占用锁，只用占用锁的进程才能进行解锁
    task_t *current_task = task_current();
    if (current_task == lock->owner)
    {
        lock->lock_count = lock->lock_count - 1;
        // 会出现进程嵌套加锁的情况，所以需要判断计数器是否变为0 如果变为0才可以释放锁
        if (lock->lock_count == 0)
        {
            lock->owner = (task_t *)0;
            
            // 如果有进程等待占用锁，那么从等待队列中获取一个进程，继续使用锁
            if (list_count(lock->wait_list) > 0)
            {
                list_node_t *task_wait_node = list_first(lock->wait_list);
                task_t *task = list_node_parent(task_wait_node, task_t, wait_node);
                list_remove(lock->wait_list, task_wait_node);
                task_set_ready(task);

                lock->owner = task;
                lock->lock_count = 1;

                task_dispach();
            }
        }
    }
    irq_leave_protection(state);
}