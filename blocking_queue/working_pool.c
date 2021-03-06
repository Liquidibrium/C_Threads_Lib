#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include "blocking_queue.h"

#define NUM_WORKERS 10

typedef struct {
  int id;
  int* count;
  BlockingQueue* q;
} Args;

void* Worker(void* args) {
  int id = ((Args*)args)->id;
  BlockingQueue* q = ((Args*)args)->q;
  while (true) {
    int x;
    BlockingQueueGetFront(q, &x);
    if (x == 0) {
      break;
    }
    //    printf("%d: %d\n", id, x);
  }
  (*((Args*)args)->count)++;
  //  printf("%d: DONE\n", id);
  return NULL;
}

int main(int argc, char** argv) {
  BlockingQueue q;
  int counter = 0;
  BlockingQueueInit(&q, sizeof(int), NULL);
  pthread_t workers[NUM_WORKERS];
  Args args[NUM_WORKERS];
  for (int i = 0; i < NUM_WORKERS; ++i) {
    args[i].id = i;
    args[i].q = &q;
    args[i].count = &counter;
    pthread_create(&workers[i], NULL, Worker, &args[i]);
  }
  for (int i = 1; i < 50; ++i) {
    BlockingQueuePushBack(&q, &i);
  }
  sleep(2);
  int x = 0;
  for (int i = 0; i < NUM_WORKERS; ++i) {
    BlockingQueuePushBack(&q, &x);
  }
  for (int i = 0; i < NUM_WORKERS; ++i) {
    pthread_join(workers[i], NULL);
  }
  BlockingQueueDestroy(&q);
  printf("result %d\n", counter == NUM_WORKERS);
  return 0;
}