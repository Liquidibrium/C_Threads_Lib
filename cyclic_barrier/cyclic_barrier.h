#ifndef __CYCLIC_BARRIER__
#define __CYCLIC_BARRIER__

#include <pthread.h>
#include <stdbool.h>

typedef void (*Function)(void*);// Barrier action  performed by the last thread entering the barrier

typedef struct CyclicBarrier {
    int number_waiting; // number of waiting threads
    int parties;        // size of barrier 
    bool broken;
    int generations;
    Function action;
    void* arg;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} CyclicBarrier;

/**
 * @brief initialization of cyclic barrier that will trip
 *        when the given number of parties (threads) are waiting upon it,
 *        and which will execute the given barrier_action when the barrier is tripped,
 *        performed by the last thread entering the barrier
 * @param barrier pointer of barrier
 * @param parties the number of threads that must invoke BarrierAwait() before the barrier is tripped
 * @param barrier_action the command to execute when the barrier is tripped, or NULL if there is no action
 * @param args fuction arguments
 */
void BarrierInit(CyclicBarrier* barrier, int parties, Function barrier_action, void* args);

/**
 * @brief destroyed memory used
 *        does not calles free(barrier)
 *        struct memory must be freed by user
 * @param barrier
 */
void BarrierDispose(CyclicBarrier* barrier);

/**
 * @brief caller thread waits until barrier is tripped or broken
 *        if current thread is last executes action function (if possible) and resets barrier
 *        returns -1 if barrier is broken or  the arrival index of the current thread,
 *        where index BarrierGetParties() - 1 indicates the first to arrive
 *        and zero indicates the last to arrive
 * @param barrier
 * @return int
 */
int BarrierAwait(CyclicBarrier* barrier);

/**
 * @brief Resets the barrier to its initial state,
 *        if any threads are waiting they are awaken
 *        with broadcast
 * @param barrier
 */
void BarrierReset(CyclicBarrier* barrier);

/**
 * @brief Returns the number of parties currently waiting at the barrier
 * @param barrier
 * @return int
 */
int BarrierGetNumberWaiting(CyclicBarrier* barrier);

/**
 * @brief Returns the number of parties required to trip this barrier
 *
 * @param barrier
 * @return int
 */
int BarrierGetParties(CyclicBarrier* barrier);

/**
 * @brief returns true if barrier
 * @param barrier
 */
bool BarrierIsBroken(CyclicBarrier* barrier);


#endif