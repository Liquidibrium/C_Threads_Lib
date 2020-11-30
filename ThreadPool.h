#ifndef __threadpool_
#define __threadpool_

#include <stdlib.h>

typedef struct {

} ThreadPool;
ThreadPool pool;

int ThreadPoolInit(ThreadPool* pool, int size);

//void ThreadPoolSchedule(ThreadPool* pool, <FUNCTION-POINTER>, <ARGUMENTS>);

int ThreadPoolShutdown(ThreadPool* pool);

     
#endif
