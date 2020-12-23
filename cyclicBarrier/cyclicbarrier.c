#include "cyclicbarrier.h"


void BarierInit(CyclicBarrier* barrier, int parties, Function barrier_action, void* args) {
    assert(barrier);
    assert(parties>0);
    barrier->parties = parties;
    barrier->number_waiting = 0;
    barrier->action = barrier_action;
    barrier->arg = args;
    if (barrier_action == NULL) {
        barrier->should_action_performed = true;
    }else{ // if action is not NULL then it should be executed 
        barrier->should_action_performed = false;
    }
    pthread_mutex_init(&barrier->lock, NULL);
    pthread_cond_init(&barrier->cond, NULL);
}

void BarierDispose(CyclicBarrier* barrier){
    assert(barrier);
    pthread_mutex_destroy(&barrier->lock);
    pthread_cond_destroy(&barrier->cond);
}

int BarierAwait(CyclicBarrier* barrier) {
    assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    barrier->number_waiting++;
    if(barrier->parties == barrier->number_waiting){
        // last thread  calls function 
        if(barrier->should_action_performed){
            barrier->should_action_performed = false;
            barrier->action(barrier->arg);
        }
        err = pthread_cond_broadcast(&barrier->cond); 
        err = pthread_mutex_unlock(&barrier->lock);
        return ;
    }

    while (barrier->number_waiting < barrier->parties ){
        err = pthread_cond_wait(&barrier->cond,&barrier->lock);
    }
   
    
    err = pthread_mutex_unlock(&barrier->lock);

    return ;
}

void BarrierReset(CyclicBarrier* barrier) {
    assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    if(barrier->action!=NULL){
        barrier->should_action_performed = true;
    }
    barrier->number_waiting = 0 ;

    err = pthread_cond_broadcast(&barrier->cond); // 

    err = pthread_mutex_unlock(&barrier->lock);
}

int BarrierGetNumberWaiting(CyclicBarrier* barrier) {
     assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    int number_waiting = barrier->number_waiting;
    err = pthread_mutex_unlock(&barrier->lock);
    return number_waiting;
}

int BarrierGetParties(CyclicBarrier* barrier) {
    assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    int parties = barrier->parties - barrier->number_waiting;
    err = pthread_mutex_unlock(&barrier->lock);
    return parties;
}

bool BarrierIsBroken(CyclicBarrier* barrier) {
    assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    bool  broken = barrier->parties == barrier->number_waiting;
    err = pthread_mutex_unlock(&barrier->lock);
    return broken;
}