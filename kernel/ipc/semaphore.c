# include "../include/ipc/semaphore.h"
#include "../include/tools/list.h"

void semaphore_init(semaphore_t *sem, int count)
{
    sem->count = count;
    list_init(&sem->wait_list);
}