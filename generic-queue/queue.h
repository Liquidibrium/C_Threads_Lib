#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stddef.h>
// if we add elem size in node it can be hetero-generic queue, but we dont need it now  
typedef struct node{
  struct node * next;
///  struct node * prev;     /// to change into  Double Ended Queue 
  void * elem;
} Node;

typedef struct {
  Node * first;
  Node * last;
  size_t elem_size;
  size_t node_count;
  void (*free_fn)(void *);
} Queue;

// Initializes queue , q pointer should not be null 
void QueueInit(Queue *q, size_t elem_size, void (*free_fn)(void *));

void QueueDispose(Queue *q);

void Enqueue(Queue *q, const void *value_ptr);

void Dequeue(Queue *q,  void *value_ptr);

void QueueFront(Queue * q, void *value_ptr);

size_t QueueSize(Queue * q );

#endif