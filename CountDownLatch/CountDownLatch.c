#include "CountDownLatch.h"
#include <assert.h>
#include <time.h>

void LatchInit(CountDownLatch* latch, int count) {
    assert(latch);
    assert(count > 0);
    latch->count = count;
    pthread_mutex_init(&latch->lock, NULL);
    pthread_cond_init(&latch->cond, NULL);
    // sem_init(&latch->sem,0,0);
}

void LatchDispose(CountDownLatch* latch) {
    assert(latch);
    int err;
    err = pthread_mutex_destroy(&latch->lock);
    err = pthread_cond_destroy(&latch->cond);
    //sem_destroy(&latch->sem);
}

void LatchAwait(CountDownLatch* latch) {
    assert(latch);
    int err;
    err = pthread_mutex_lock(&latch->lock);
    if (latch->count == 0) {
        err = pthread_mutex_unlock(&latch->lock);
        return;
    }

    err = pthread_cond_wait(&latch->cond, &latch->lock);

    // VS SECOND VERSION only semaphore 
    // int count = latch->count;
    // for(int i = 0; i < count ; i++){
    //     sem_wait(&latch->sem);
    // }
}

int LatchTimedAwait(CountDownLatch* latch, long time_to_wait) {
    assert(latch);
    int err;
    err = pthread_mutex_lock(&latch->lock);
    if (latch->count == 0) {
        err = pthread_mutex_unlock(&latch->lock);
        return  err;
    }
    struct timespec ts;
    err = clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += time_to_wait;
    if (time_to_wait == 0 ||
        (err = pthread_cond_timedwait(&latch->cond, &latch->lock, &ts)) == -1) { // Timed out 
        // printf("errno %d\n",errno);
        return err;
    }
    return err;
}
void LatchCountDown(CountDownLatch* latch) {
    assert(latch);
    int err;
    err = pthread_mutex_lock(&latch->lock);
    if (latch->count == 0) {
        err = pthread_mutex_unlock(&latch->lock);
        return;
    }

    latch->count--;                                          // CAS?

    if (latch->count == 0)
        err = pthread_cond_broadcast(&latch->cond);               // signal? 
    err = pthread_mutex_unlock(&latch->lock);

    // VS SECOND VERSION only semaphore 
    // int value;
    // sem_getvalue(&latch->sem,&value);
    // if(value<=latch->count);
    // sem_post(&latch->sem);
}


int LatchGetCount(CountDownLatch* latch) {
    assert(latch);
    int err;
    err = pthread_mutex_lock(&latch->lock);
    int count = latch->count;
    err = pthread_mutex_unlock(&latch->lock);
    return count;

    // VS SECOND VERSION
    // int value;
    // sem_getvalue(&latch->sem,&value);
    // return latch->count - value;
}