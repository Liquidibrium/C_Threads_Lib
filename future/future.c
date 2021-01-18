#include "future.h"

#include <assert.h>
#include <stdlib.h>
#include <errno.h>

// not used 
#define handle_error_en(en, msg) \
         errno = en; perror(msg); exit(EXIT_FAILURE);


void* thread_fun(void* t_args) {
    Future* fut = (Future*)t_args;
    fut->result = fut->func(fut->argument);
    fut->done = true;
    sem_post(&fut->sem);
    return NULL;
}

void FutureInit(Future* fut, Function func, void* arg) {
    assert(fut);
    assert(func);
    fut->func = func;
    fut->argument = arg;
    fut->cancelled = false;
    fut->done = false;
    sem_init(&fut->sem, 0, 0);
    pthread_create(&fut->thread, NULL, thread_fun, fut);
}

void FutureDispose(Future* fut) {
    assert(fut);
    sem_destroy(&fut->sem);
}

int FutureCancel(Future* fut) {
    assert(fut);
    int err;
    if (!fut->cancelled) {
        if (fut->done) {
            // already done 
            return -1;
        }
        err = pthread_cancel(fut->thread);
        // if(err != 0 )
        //     handle_error_en(s, "pthread_cancel");
        void* status;
        err = pthread_join(fut->thread, &status);
        // if (s != 0)
        //     handle_error_en(s, "pthread_join");
        fut->cancelled = (status == PTHREAD_CANCELED);
    }
    else { // already cancelled
        err = -1;
    }
    return err;
}

//Returns true if this task was cancelled before it is done 
bool FutureIsCancelled(Future* fut) {
    assert(fut);
    bool cancelled = fut->cancelled;
    return cancelled;
}
//return true if task is completed
bool FutureIsDone(Future* fut) {
    assert(fut);
    bool done = fut->done;
    return done;
}

int FutureTimedGetResult(Future* fut, void** value_pointer, long time_to_wait) {
    assert(fut);
    if (fut->done || fut->cancelled) {// already done  or Cancelled 
        return -1;
    }
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += time_to_wait;
    int err;
    if (time_to_wait == 0 ||
        (err = sem_timedwait(&fut->sem, &ts)) == -1) { // Timed out 
       // printf("errno %d\n",errno);
        return -1;
    }
    *value_pointer = fut->result;
    return 0;
}

int FutureGetResult(Future* fut, void** value_pointer) {
    assert(fut);
    assert(value_pointer);
    if (fut->done || fut->cancelled) {// already done  or Cancelled 
        return -1;
    }
    int err;
    void* status;
    err = pthread_join(fut->thread, &status);
    if (err != 0)return err;
    //vs
    // err = sem_wait(&fut->sem);
    *value_pointer = fut->result;
    return err;
}


