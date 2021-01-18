#include "ThreadPool.h"
#include <assert.h>
#include <stdio.h>

typedef struct {         /* struct  Task stored in blockingQueue */
    bool endWorking;     /* if true running thread that's signal to stop */
    void (*func)(void*); /* function to execute */
    void* args;          /* function arguments */
} Task;


void* working_fn(void* arg) {
    ThreadPool* tPool = (ThreadPool*)arg;
    //pthread_t t = pthread_self();
    while (true) {
        Task currTask;
        int err = BlockingQueueTryGetFront(&tPool->bq, &currTask);
        if (err == 0) {
            if (currTask.endWorking) // end of this thread
                break;
            currTask.func(currTask.args);
        }
    }
    return NULL;
}

int ThreadPoolInit(ThreadPool* tpool, size_t size) {
    assert(tpool);
    assert(size > 0);
    tpool->running = true;
    tpool->size = size;
    BlockingQueueInit(&tpool->bq, sizeof(Task), NULL);
    tpool->pthreads = malloc(size * sizeof(pthread_t));
    assert(tpool->pthreads);
    for (size_t i = 0; i < size; i++) {
        pthread_create(tpool->pthreads + i, NULL, working_fn, tpool);
    }
    return 0;
}

int ThreadPoolSchedule(ThreadPool* tpool, void (*func)(void*), void* args) {
    assert(tpool);
    assert(func);
    if (!tpool->running) { //should be finished
        return -1;
    }
    Task nextTask;
    nextTask.func = func;
    nextTask.args = args;
    nextTask.endWorking = false;
    BlockingQueuePushBack(&tpool->bq, &nextTask);
    return 0;
}

int ThreadPoolWait(ThreadPool* tpool, bool immediate_shutDown) {
    if (tpool == NULL)
        return -1;
    tpool->running = false;
    for (size_t i = 0; i < tpool->size; ++i) {
        Task endTask;
        endTask.endWorking = true;
        if (immediate_shutDown) {
            BlockingQueuePushFront(&tpool->bq, &endTask);
        }
        else {
            BlockingQueuePushBack(&tpool->bq, &endTask);
        }
    }
    // BlockingQueueShutDown(&tpool->bq);
    for (size_t i = 0; i < tpool->size; ++i) {
        pthread_join((tpool->pthreads[i]), NULL);
    }
    return 0;
}

void Threads_cancel(ThreadPool* tpool) {
    for (size_t i = 0; i < tpool->size; ++i) {
        pthread_cancel(tpool->pthreads[i]);
    }
    for (size_t i = 0; i < tpool->size; ++i) {
        pthread_join((tpool->pthreads[i]), NULL);
    }
}

int ThreadPoolShutdown(ThreadPool* tpool) {
    if (tpool == NULL)
        return -1; // shouldnot be null
    if (tpool->running) {
        //ThreadPoolWait(tpool,true);
        // OR 
        Threads_cancel(tpool);
    }
    BlockingQueueDestroy(&tpool->bq);
    free(tpool->pthreads);
    return 0;
}


// worker function version 1.0 not working (yet)
void* working_fn2(void* arg) {
    ThreadPool* tPool = (ThreadPool*)arg;
    // pthread_t t = pthread_self();
    while (true) {
        if (!tPool->running)
            break;
        Task currTask;
        int err = BlockingQueueTryGetFront(&tPool->bq, &currTask);
        if (err == 0) {
            void (*func)(void*) = currTask.func;
            void* args = currTask.args;
            func(args);
        }
        else if (err == 1) { // bq shutdowned 
            break;
        }
    }
    //pthread_exit(NULL);
    return NULL;
}

