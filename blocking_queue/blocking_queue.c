#include "blocking_queue.h"
#include "errno.h"
#include <assert.h>

void BlockingQueueInit(BlockingQueue* bq, int elem_size,
  void (*free_fn)(void*)) {
  bq->cancelled = false; // used only for try get front elements, can be added to others ... 
  LinkedQueueInit(&bq->que, elem_size, free_fn);
  pthread_mutex_init(&bq->lock, NULL);
  pthread_cond_init(&bq->cond, NULL);
}

void BlockingQueueDestroy(BlockingQueue* bq) {
  LinkedQueueDispose(&bq->que);
  pthread_mutex_destroy(&bq->lock);
  pthread_cond_destroy(&bq->cond);
}

void BlockingQueuePushBack(BlockingQueue* bq, void* value_ptr) {
  pthread_mutex_lock(&bq->lock);
  LinkedQueuePushBack(&bq->que, value_ptr);
  pthread_cond_signal(&bq->cond);
  // if (QueueSize(&bq->que) == 1){ // can be change with Broadcast;
  //   pthread_cond_broadcast(&bq->cond);
  // }
  pthread_mutex_unlock(&bq->lock);
}

void BlockingQueuePushFront(BlockingQueue* bq, void* value_ptr) {
  pthread_mutex_lock(&bq->lock);
  LinkedQueuePushFront(&bq->que, value_ptr);
  pthread_cond_signal(&bq->cond);
  // if (QueueSize(&bq->que) == 1){ // can be change with Broadcast;
  //   pthread_cond_broadcast(&bq->cond);
  // }
  pthread_mutex_unlock(&bq->lock);
}

void BlockingQueueGetFront(BlockingQueue* bq, void* value_ptr) {
  pthread_mutex_lock(&bq->lock);
  while (LinkedQueueIsEmpty(&bq->que)) { // Queue is empty
    pthread_cond_wait(&bq->cond, &bq->lock);
  }
  LinkedQueueGetFront(&bq->que, value_ptr);
  pthread_mutex_unlock(&bq->lock);
}

int BlockingQueueTryGetFront(BlockingQueue* bq, void* value_ptr) {
  int err = pthread_mutex_trylock(&bq->lock);
  if (err == 0) {
    while (LinkedQueueIsEmpty(&bq->que)) { // Queue is empty
      pthread_cond_wait(&bq->cond, &bq->lock);
      if (bq->cancelled && LinkedQueueIsEmpty(&bq->que)) {
        pthread_mutex_unlock(&bq->lock);
        return 1; // cancelled
      }
    }
    LinkedQueueGetFront(&bq->que, value_ptr);
    pthread_mutex_unlock(&bq->lock);
    return 0;
  }
  return err;
}

void BlockingQueueShutDown(BlockingQueue* bq) {
  pthread_mutex_lock(&bq->lock);
  bq->cancelled = true;
  pthread_cond_broadcast(&bq->cond);
  pthread_mutex_unlock(&bq->lock);
}

int BlockingQueueTimedGet(BlockingQueue* bq, void* value_ptr, long time_to_wait) {
  struct timespec ts;
  pthread_mutex_lock(&bq->lock);
  while (LinkedQueueIsEmpty(&bq->que)) { // Queue is empty
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += time_to_wait;
    if (time_to_wait == 0 ||
      pthread_cond_timedwait(&bq->cond, &bq->lock, &ts) == -1) {
      return ETIMEDOUT;
    }
  }
  LinkedQueueGetFront(&bq->que, value_ptr);
  pthread_mutex_unlock(&bq->lock);
  return 0;
}

size_t BlockingQueueSize(BlockingQueue* bq) {
  pthread_mutex_lock(&bq->lock);
  size_t size = LinkedQueueSize(&bq->que);
  pthread_mutex_unlock(&bq->lock);
  return size;
}

bool BlockingQueueIsEmpty(BlockingQueue* bq) {
  pthread_mutex_lock(&bq->lock);
  bool empty = LinkedQueueIsEmpty(&bq->que);
  pthread_mutex_unlock(&bq->lock);
  return empty;
}