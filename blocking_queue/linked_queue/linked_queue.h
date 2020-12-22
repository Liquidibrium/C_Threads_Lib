#ifndef _QUEUE_H_
#define _QUEUE_H_
#include <stddef.h>
#include <stdbool.h>
// if we add elem size in node it can be hetero-generic queue, but we dont need it now  
typedef struct node{
  struct node * next;
///  struct node * prev;     /// to change into  Double Ended Queue 
  void * elem;
} Node;

typedef struct Queue {
  Node * first; // pointer to front node 
  Node * last;  // pointer to last node 
  size_t elem_size; // element size in queue 
  size_t node_count;  // number of element in queue
  void (*free_fn)(void *); // free function for element, is used in dispose if is not NULL
} LinkedQueue;

/**
 * @brief Initializes Generic type FIFO Queue
 *        
 * @param que should not be NULL, should pointing memory area sizeof(Queue)
 * @param elem_size  size of add elements in queue 
 * @param free_fn   QueueDispose takes care of memory area of element if added param in queue is pointer type
 */
int LinkedQueueInit(LinkedQueue *que, size_t elem_size, void (*free_fn)(void *));

/**
 * @brief takes care of memory area of element 
 *        if added param in queue is pointer type
 *        and free_fn is not NULL 
 * 
 * @param que 
 */
int LinkedQueueDispose(LinkedQueue *que);

/**
 * @brief adds value_ptr in Queue 
 * 
 * @param que 
 * @param value_ptr 
 */
int LinkedQueuePushBack(LinkedQueue *que, const void *value_ptr);

/**
 * @brief deletes front element from Queue 
 *        and returns if in value_prt
 *        passes ownership of that element to user of Queue 
 * @param que 
 * @param value_ptr 
 */
int LinkedQueueGetFront(LinkedQueue *que,  void *value_ptr);
/**
 * @brief returns front element from Queue 
 *        the element should not be freed 
 *        if it is not deqeued from queue 
 *        not passes ownership of that element 
 * @param que 
 * @param value_ptr 
 */
int LinkedQueuePeekFront(LinkedQueue * que, void *value_ptr);

/**
 * @brief returns number of elements in Queue
 * 
 * @param que 
 * @return size_t 
 */
size_t LinkedQueueSize(LinkedQueue * que);

/**
 * @brief adds element in queue from front 
 * 
 * @param que 
 * @param value_ptr 
 * @return int 
 */
int LinkedQueuePushFront(LinkedQueue* que, const void *value_ptr);

/**
 * @brief returns true if que is empty
 * 
 * @param que 
 * @return true 
 * @return false 
 */
bool LinkedQueueIsEmpty(LinkedQueue *que);

/**
 * @brief return last element value
 *      but not removes it from queue
 * @param que 
 * @param value_ptr 
 * @return int 
 */
int  LinkedQueuePeekBack(LinkedQueue * que, void *value_ptr);



// not implemented, can be easili implemented with double linked
int  LinkedQueueGetBack(LinkedQueue *que,  void *value_ptr);




#endif