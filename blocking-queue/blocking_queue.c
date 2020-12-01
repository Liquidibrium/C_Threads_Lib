#include "blocking_queue.h"

void BlockingQueueInit(BlockingQueue *bq, int elem_size,
                       void (*free_fn)(void *)) {
  QueueInit(&bq->que, elem_size, free_fn);
  pthread_mutex_init(&bq->lock, NULL);
  pthread_cond_init(&bq->cond, NULL);
}

void BlockingQueueDestroy(BlockingQueue *bq) {
  QueueDispose(&bq->que);
  pthread_mutex_destroy(&bq->lock);
  pthread_cond_destroy(&bq->cond);
}

void BlockingQueueAdd(BlockingQueue *bq, void *value_ptr) {
  pthread_mutex_lock(&bq->lock);
  Enqueue(&bq->que, value_ptr);
  pthread_cond_signal(&bq->cond);
  pthread_mutex_unlock(&bq->lock);
}

void BlockingQueueGet(BlockingQueue *bq, void *value_ptr) {
  pthread_mutex_lock(&bq->lock);
  while (QueueSize(&bq->que) == 0) {
    pthread_cond_wait(&bq->cond, &bq->lock);
  }
  Dequeue(&bq->que, value_ptr);
  pthread_mutex_unlock(&bq->lock);
}