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

#include <pthread.h>

#include "../generic-queue/queue.h"

typedef struct {
  Queue que;              // int
  pthread_mutex_t lock; // 1
  pthread_cond_t cond;
} BlockingQueue;

void BlockingQueueInit(BlockingQueue *q, int elem_size,
                       void (*free_fn)(void *));

void BlockingQueueDestroy(BlockingQueue *q);

void BlockingQueueAdd(BlockingQueue *q, void *value_ptr);

void BlockingQueueGet(BlockingQueue *q, void *value_ptr);