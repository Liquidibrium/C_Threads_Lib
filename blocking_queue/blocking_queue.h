/* Add  -- add new element */
/*          signal */
/* Get  -- takes out one element */
/*          if queue is empty */
/* 	    wait until there is at least one element */

/* Producer/Consumer - FIFO queue */
/*   - First In First Out */
/* Stack - LIFO queue */
/*   - Last In First Out */

/* 1. register listener on cond object */
/* 2. unlock mutex */
/* ... */
/* ... wait for signal */
/* ... */
/* 3. lock mutex again */
/* 4. return from wait */

/* 1. send signal */
/* 2. unlock mutex */
#ifndef __blockingqueue_
#define __blockingqueue_

#include <pthread.h>
#include <stdbool.h>
#include "./linked_queue/linked_queue.h"


typedef struct {
  LinkedQueue que;          
  pthread_mutex_t lock; 
  pthread_cond_t cond;
  bool cancelled; // if true blocking queue should stop 
} BlockingQueue;

void BlockingQueueInit(BlockingQueue *q, int elem_size,
                       void (*free_fn)(void *));

void BlockingQueueDestroy(BlockingQueue *q);

void BlockingQueuePushBack(BlockingQueue *q, void *value_ptr);

void BlockingQueuePushFront(BlockingQueue *bq, void *value_ptr);

void BlockingQueueGetFront(BlockingQueue *q, void *value_ptr);

int BlockingQueueTryGetFront(BlockingQueue *bq, void *value_ptr);

size_t BlockingQueueSize(BlockingQueue *q);

bool BlockingQueueIsEmpty(BlockingQueue *q);

int BlockingQueueTimedGet(BlockingQueue *bq, void * value_ptr, long time_to_wait);

void BlockingQueueShutDown(BlockingQueue *bq);

#endif