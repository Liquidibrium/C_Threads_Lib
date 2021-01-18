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

/**
 * @brief initialize blocking queue  with linked queue
 *
 * @param q pointer should be valid
 * @param elem_size element size in queue
 * @param free_fn if element is pointer free fn is needed
 */
void BlockingQueueInit(BlockingQueue* q, int elem_size,
  void (*free_fn)(void*));

/**
 * @brief frees memory space of  blocking queue
 * @param q
 */
void BlockingQueueDestroy(BlockingQueue* q);

/**
 * @brief adds element in end of the queue
 * @param q
 * @param value_ptr
 */
void BlockingQueuePushBack(BlockingQueue* q, void* value_ptr);

/**
 * @brief adds element in front of the queue
 *
 * @param bq
 * @param value_ptr
 */
void BlockingQueuePushFront(BlockingQueue* bq, void* value_ptr);

/**
 * @brief gets front element of the queue
 * @param q
 * @param value_ptr pointer of first element stored there
 */
void BlockingQueueGetFront(BlockingQueue* q, void* value_ptr);

/**
 * @brief tries to get front elements from blocking queue 
 *        uses mutex try lock function 
 * @param bq 
 * @param value_ptr return 
 * @return int 
 */
int BlockingQueueTryGetFront(BlockingQueue* bq, void* value_ptr);

/**
 * @brief return current size of blocking queue
 * @param q
 * @return size_t
 */
size_t BlockingQueueSize(BlockingQueue* q);

/**
 * @param q
 * @return true if is empty
 */
bool BlockingQueueIsEmpty(BlockingQueue* q);

/**
 * @brief waits blocking queue to get
 *         first elements from queue
 *         whithin the time passed
 * @param bq
 * @param value_ptr returned value pointer
 * @param time_to_wait  time in seconds
 * @return int  error code
 */
int BlockingQueueTimedGet(BlockingQueue* bq, void* value_ptr, long time_to_wait);

/**
 * @brief broadcasts all blocked threads and sets cancelled as true
 *
 * @param bq
 */
void BlockingQueueShutDown(BlockingQueue* bq);

#endif