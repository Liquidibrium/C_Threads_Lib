#include "blocking_queue.h"

void BlockingQueueInit(BlockingQueue *q, int elem_size,
                       void (*free_fn)(void *)) {
  QueueInit(&q->que, elem_size, free_fn);
  pthread_mutex_init(&q->lock, NULL);
  pthread_cond_init(&q->cond, NULL);
}

void BlockingQueueDestroy(BlockingQueue *q) {
  QueueDispose(&q->que);
  pthread_mutex_destroy(&q->lock);
  pthread_cond_destroy(&q->cond);
}

void BlockingQueueAdd(BlockingQueue *q, void *value_ptr) {
  pthread_mutex_lock(&q->lock);
  Enqueue(&q->que, value_ptr);
  pthread_cond_signal(&q->cond);
  pthread_mutex_unlock(&q->lock);
}

void BlockingQueueGet(BlockingQueue *q, void *value_ptr) {
  pthread_mutex_lock(&q->lock);
  if (q->que.log_len == 0) {
    pthread_cond_wait(&q->cond, &q->lock);
  }
  Dequeue(&q->que, value_ptr);
  pthread_mutex_unlock(&q->lock);
}