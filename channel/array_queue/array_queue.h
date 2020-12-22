#ifndef _ARR_QUEUE_H_
#define _ARR_QUEUE_H_
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
typedef  unsigned int  uint_t; 

typedef struct Queue {
  size_t  first; // index of first element
  size_t  last; // index of last element
  size_t  elem_size; // sizeof element in array 
  size_t  elem_count;  // number of elements in 
  size_t  capacity;  // array capacity  
  void  * elems;  // array of elements 
  void (*free_fn)(void *); // free function of element 
} ArrayQueue;


void ArrayQueueInit(ArrayQueue *q,size_t capacity, size_t elem_size, void (*free_fn)(void *));

int ArrayQueueDispose(ArrayQueue *q);

int ArrayQueuePushBack(ArrayQueue *q, const void *value_ptr);

int ArrayQueuePushFront(ArrayQueue* q, void *value_ptr);

int  ArrayQueueGetFront(ArrayQueue *q,  void *value_ptr);

int  ArrayQueueGetBack(ArrayQueue *q,  void *value_ptr);

size_t ArrayQueueSize(ArrayQueue * q);

bool ArrayQueueIsFull(ArrayQueue *q);

bool ArrayQueueIsEmpty(ArrayQueue *q);

int  ArrayQueuePeekFront(ArrayQueue * q, void *value_ptr);

int  ArrayQueuePeekBack(ArrayQueue * q, void *value_ptr);


#endif