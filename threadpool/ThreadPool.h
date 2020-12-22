#ifndef __threadpool_
#define __threadpool_

#include <stdlib.h>
#include "../blocking-queue/blocking_queue.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct {
    BlockingQueue bq;
    pthread_t* pthreads;
    size_t size; 
    bool running; // 0 means should be Shut downed, 1 means treadPool is working 
} ThreadPool;

ThreadPool pool;

    /**
     * @brief initialize thread pool 
     * @param tpool 
     * @param size 
     * @return int 
     */
int ThreadPoolInit(ThreadPool* tpool, size_t size);
    
    /**
     * @brief add new Task in tpool
     * @param tpool 
     * @param func 
     * @param args 
     * @return  
     */
int ThreadPoolSchedule(ThreadPool* tpool, void (*func)(void *),void* args );


/**
 * @brief imediately finishes work 
 *        and destorys tpool
 * @param tpool 
 * @return int 
 */
int ThreadPoolShutdown(ThreadPool* tpool);
     
int ThreadPoolWait(ThreadPool *tpool, bool imediate);
#endif
