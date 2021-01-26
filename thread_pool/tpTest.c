#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

#include "thread_pool.h"

int left;
pthread_mutex_t lock;


void Test_task(void *arg) {
   // usleep(100);
    pthread_mutex_lock(&lock);
    left--;
    printf("left: %d Task: %ld\n",left,(long)arg);
    pthread_mutex_unlock(&lock);
}

int main(int argc, char **argv)
{
    long i;
    int THREAD = 30;
    int SIZE = 100;
    pthread_mutex_init(&lock, NULL);

    ThreadPool pool;
    left = SIZE;
    ThreadPoolInit(&pool,THREAD);
    for (i = 0; i < SIZE; i++) {
        assert(ThreadPoolSchedule(&pool, &Test_task,(void*) i) == 0);
    }
    
    ThreadPoolWait(&pool,false);
    assert(ThreadPoolShutdown(&pool) == 0);
    printf("result %d\n", left == 0);
    pthread_mutex_destroy(&lock);

    return 0;
}