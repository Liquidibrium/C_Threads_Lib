#ifndef __future_
#define __future_

#include <stdbool.h>
#include <semaphore.h>
#include <pthread.h>

typedef void* (*Function)(void* arguments); // passed function in FutureInit

typedef struct Future {
    pthread_t thread; // thread id in which function is running
    Function func;    // passed function to execute
    void* argument;   // passed function argument
    void* result;     // function returned result
    bool cancelled;   // indicates if thread is cancelled
    bool done;        // indicates if function finished
    sem_t sem;        // semaphore to use timedwait
} Future;

/**
 * @brief initialize future
 *
 * @param fut should have memory allocated by user
 * @param func fuction to execute
 * @param arg function arguments
 */
void FutureInit(Future* fut, Function func, void* arg);

/**
 * @param fut
 * @param value_pointer void** to return (void*) result in *(value_pointer)
 * @return int returns 0 if succeed otherwise -1
 */
int FutureGetResult(Future* fut, void** value_pointer);

/**
 * @brief destroyed memory used
 *        does not calles free(fut)
 *        struct memory must be freed by user
 * @param fut
 */
void FutureDispose(Future* fut);

/**
 * @brief  tries to cancel future running thread
 *         returns 0 if succeed else -1 if already cancelled/done
 *         or thread error occurred
 * @param fut
 * @return int
 */
int FutureCancel(Future* fut);

/**
 * @brief tries to get result within passed time as argument
 *        returns -1 if future is already done/cancelled or timedout
 *        sem_timedwait error can be detected using errno in errno.h
 * @param fut
 * @param value_pointer  if succeed return value pointer
 * @param time_to_wait
 * @return int
 */
int FutureTimedGetResult(Future* fut, void** value_pointer, long time_to_wait);

/**
 * @brief Returns true if this task was cancelled before it is done
 *
 * @param fut
 * @return true
 * @return false
 */
bool FutureIsCancelled(Future* fut);

/**
 * @brief return true if task is completed
 *
 * @param fut
 * @return true
 * @return false
 */
bool FutureIsDone(Future* fut);
#endif