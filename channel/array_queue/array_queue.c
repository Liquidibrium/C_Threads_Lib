
#include "array_queue.h"
#include <stdio.h>
#define GET_ELEM_ADDRESS(q, index) ((char *)q->elems + (index)*q->elem_size)

void ArrayQueueInit(ArrayQueue *q, size_t capacity, size_t elem_size, void (*free_fn)(void *))
{
    assert(q);
    assert(capacity >= 0); // if 0 ...
    assert(elem_size > 0);
    q->capacity = capacity;
    q->free_fn = free_fn;
    q->elem_size = elem_size;
    q->elems = malloc(capacity * elem_size);
    assert(q->elems);
    q->first = 0;
    q->last = -1;
    q->elem_count = 0;
}
int ArrayQueueDispose(ArrayQueue *q)
{

    assert(q);
    if (q->free_fn != NULL)
    {
        if (q->elem_count == 0)
        {
            goto fr;
        }
        if (q->first <= q->last)
        {
            for (size_t i = q->first; i <= q->last; i++)
            {
                q->free_fn(GET_ELEM_ADDRESS(q, i));
            }
        }
        else if (q->last < q->first)
        {
            for (size_t i = q->first; i < q->capacity; i++)
            {
                q->free_fn(GET_ELEM_ADDRESS(q, i));
            }
            for (size_t i = 0; i <= q->last; i++)
            {
                q->free_fn(GET_ELEM_ADDRESS(q, i));
            }
        }
    }
fr:
    free(q->elems);
    return 0;
}

// takes ownership of value_ptr
int ArrayQueuePushBack(ArrayQueue *q, const void *value_ptr)
{
    assert(q);
    assert(value_ptr);
    if (q->elem_count == q->capacity) // Queue is FULL
        return -1;
    q->last = (q->last + 1) % q->capacity; //now if q->last+1 == q->capacity then empty slot is at the begining ;
    memcpy(GET_ELEM_ADDRESS(q, q->last), value_ptr, q->elem_size);
    q->elem_count++;
    return 0; // success
}

// dequeues from front in value_prt , if queue is empty then value_ptr is unchanged and returned -1;
int ArrayQueueGetFront(ArrayQueue *q, void *value_ptr)
{
    assert(q);
    assert(value_ptr);
    if (q->elem_count == 0)
        return -1; // get element from empty queue
    memcpy(value_ptr, GET_ELEM_ADDRESS(q, q->first), q->elem_size);
    q->first = (q->first + 1) % q->capacity; // same as add
    q->elem_count--;
    return 0; // succes;
}

size_t ArrayQueueSize(ArrayQueue *q)
{
    assert(q);
    return q->elem_count;
}

int ArrayQueuePeekFront(ArrayQueue *q, void *value_ptr)
{
    assert(q);
    assert(value_ptr);
    if (q->elem_count == 0)
        return -1; // get element from empty queue
    memcpy(value_ptr, GET_ELEM_ADDRESS(q, q->first), q->elem_size);
    return 0; // success
}

// implement later
int ArrayQueuePushFront(ArrayQueue *q, void *value_ptr)
{
    assert(q);
    assert(value_ptr);
    if (q->elem_count == q->capacity)
        return -1; // Queue is FULL
    if (q->first == 0)
    {
        q->first = q->capacity - 1;
    }
    else
    {
        q->first--;
    }
    memcpy(GET_ELEM_ADDRESS(q, q->first), value_ptr, q->elem_size);
    q->elem_count++;
    return 0; // success
}

int ArrayQueueGetBack(ArrayQueue *q, void *value_ptr)
{
    assert(q);
    assert(value_ptr);
    if (q->elem_count == 0)
        return -1; // get element from empty queue
    memcpy(value_ptr, GET_ELEM_ADDRESS(q, q->last), q->elem_size);
    if (q->last == 0)
        q->last = q->capacity - 1;
    else
        q->last--;
    q->elem_count--;
    return 0; // succes;
}

int ArrayQueuePeekBack(ArrayQueue *q, void *value_ptr)
{
    assert(q);
    assert(value_ptr);
    if (q->elem_count == 0)
        return -1; // get element from empty queue
    memcpy(value_ptr, GET_ELEM_ADDRESS(q, q->last), q->elem_size);
    return 0;
}

bool ArrayQueueIsFull(ArrayQueue *q)
{
    assert(q);
    return q->elem_count == q->capacity;
}

bool ArrayQueueIsEmpty(ArrayQueue *q)
{
    assert(q);
    return q->elem_count == 0;
}