#include "queue.h"

#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

void QueueInit(Queue* q, size_t elem_size, void (*free_fn)(void *)) {
  assert(q);
  assert(elem_size > 0);
  q->first = NULL;
  q->last = NULL;
  q->node_count = 0;
  q->elem_size = elem_size;
  q->free_fn = free_fn;
}

void QueueDispose(Queue* q) {
  if(q == NULL)return; // nothing to be freed 
  while(q->first){
    Node * tmp = q->first;
    q->first = q->first->next;
    if(q->free_fn){
      q->free_fn(tmp->elem);
    }
    free(tmp->elem);
    free(tmp);
  }
}
/// add element at the end 
void Enqueue(Queue* q, const void *value_ptr) {
      assert(q != NULL);
      Node * newNode = malloc(sizeof(struct node));
      assert(newNode);
      newNode->elem = malloc(q->elem_size);
      assert(newNode->elem);
      memcpy(newNode->elem, value_ptr, q->elem_size);
      newNode->next = NULL;
    if(q->first == NULL) {  // q->node_count  == 0 ;(isEmpty)
    //newNode->prev = NULL;
      q->first = newNode;
      q->last = newNode;
    } else {
      q->last->next = newNode;
    //newNode->prev = q->last; 
      q->last = q->last->next; // newNode 
    }
    q->node_count++;
}

void QueueFront(Queue * q, void *value_ptr) {
	assert(q != NULL);
  assert(q->node_count > 0);
  memcpy(value_ptr, q->first->elem, q->elem_size);
}

// gets first element 
void Dequeue(Queue* q, void *value_ptr) {
  assert(q != NULL);
  assert(q->node_count > 0);
  memcpy(value_ptr, q->first->elem, q->elem_size);
  if(q->node_count == 1) { // only one element in que
    free(q->first->elem);
  //printf("\nhere\n");
    free(q->first); 
    q->first = NULL;
    q->last = NULL;
  } else {
    Node * tmp = q->first;
    q->first = q->first->next;
  //q->first->prev = NULL;
    free(tmp->elem);
    free(tmp);
  }
  q->node_count--;
}

size_t QueueSize(Queue * q){
  assert(q != NULL);
  return  q->node_count; 
}
