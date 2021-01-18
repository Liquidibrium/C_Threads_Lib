#ifndef __CYCLIC_BARRIER__
#define __CYCLIC_BARRIER__

#include <pthread.h>
#include <stdbool.h>

typedef void (*Function)(void*);// Barrier action  performed by the last thread entering the barrier

typedef struct CyclicBarrier {
    int number_waiting; // number of waiting threads
    int parties;        // size of barrier 
    bool should_action_performed;

    Function action;
    void* arg;
    pthread_mutex_t lock;
    pthread_cond_t cond;
} CyclicBarrier;

void BarierInit(CyclicBarrier* barrier, int parties, Function barrier_action, void* args);

void BarierDispose(CyclicBarrier* barrier);

int BarierAwait(CyclicBarrier* barrier);

void BarrierReset(CyclicBarrier* barrier);

int BarrierGetNumberWaiting(CyclicBarrier* barrier);

int BarrierGetParties(CyclicBarrier* barrier);

bool BarrierIsBroken(CyclicBarrier* barrier);


#endif