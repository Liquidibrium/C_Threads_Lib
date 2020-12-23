#ifndef __COUNT_DOWN_LATCH__
#define __COUNT_DOWN_LATCH__

//#include <semaphore.h>
#include <pthread.h>

typedef struct CountDownLatch {
    pthread_mutex_t lock;
    pthread_cond_t cond;
    int count; // number of threads that should CountDown to get zero 

    //sem_t sem; // can be written using only count without lock and cond 
} CountDownLatch;

/**
 * @brief initialize latch
 * 
 * @param latch 
 * @param count as thread counter
 */
void LatchInit(CountDownLatch* latch, int count);

/**
 * @brief destroies memory used 
 *        does not callse free(latch)
 *        struct memory must be freed by user
 * @param latch 
 */
void LatchDispose(CountDownLatch* latch);

/**
 * @brief the thread whih calls this method 
 *        waits until count becames zero and 
 *        then continues run  
 * @param latch 
 */
void LatchAwait(CountDownLatch* latch);

/**
 * @brief the thread which calls this method 
 *        waits until count becames zero or 
 *        until time_to_wait as seconds  
 *        then continues run 
 * 
 * @param latch 
 * @param time_to_wait 
 * @return int return -1 if E TIMED OUT , 0 if latch count is already 0 or timedwaited successfully 
 */
int LatchTimedAwait(CountDownLatch* latch, long time_to_wait);

/**
 * @brief decrement count by one 
 *        if LatchAwait is called
 *        then  the waiting thread is realesed 
 * @param latch 
 */
void LatchCountDown(CountDownLatch* latch);

/**
 * @brief  return value of count 
 *         at that specific moment, but can change every time;
 * @param latch 
 * @return int 
 */
int LatchGetCount(CountDownLatch* latch);
#endif