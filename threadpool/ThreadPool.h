#ifndef __threadpool_
#define __threadpool_

#include <stdlib.h>
#include "../blocking_queue/blocking_queue.h"
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
     * @brief add new Task in thread pool
     * @param tpool 
     * @param func 
     * @param args 
     * @return  
     */
int ThreadPoolSchedule(ThreadPool* tpool, void (*func)(void *),void* args );

/**
 * @brief caller thread waits thread pool to finish 
 *      
 * @param tpool 
 * @param immediate if false waits all tasks to be done else tries to cancel immediately  
 * @return int 
 */
int ThreadPoolWait(ThreadPool *tpool, bool immediate);

/**
 * @brief immediately finishes work 
 *        and destroys thread pool
 * @param tpool 
 * @return int 
 */
int ThreadPoolShutdown(ThreadPool* tpool);
     
#endif
