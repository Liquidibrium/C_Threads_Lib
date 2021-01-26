#include "cyclic_barrier.h"
#include <assert.h>

int reset_barrier(CyclicBarrier* barrier, bool flag) {
    int err = pthread_cond_broadcast(&barrier->cond);
    barrier->number_waiting = 0;
    barrier->broken = flag;
    barrier->generations++;
    return err;
}

void BarrierInit(CyclicBarrier* barrier, int parties, Function barrier_action, void* args) {
    assert(barrier);
    assert(parties > 0);
    barrier->parties = parties;
    barrier->number_waiting = 0;
    barrier->action = barrier_action; // if action is not NULL then it should be executed
    barrier->arg = args;

    barrier->broken = false;
    barrier->generations = 0;
    pthread_mutex_init(&barrier->lock, NULL);
    pthread_cond_init(&barrier->cond, NULL);
}

void BarrierDispose(CyclicBarrier* barrier) {
    assert(barrier);
    pthread_mutex_destroy(&barrier->lock);
    pthread_cond_destroy(&barrier->cond);
}

int BarrierAwait(CyclicBarrier* barrier) {
    assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    if (barrier->broken) {
        err = pthread_mutex_unlock(&barrier->lock);
        return -1;
    }
    int curr_gen = barrier->generations;
    barrier->number_waiting++;
    int index = barrier->parties - barrier->number_waiting;
    if (barrier->parties == barrier->number_waiting) {
        // last thread  calls function 
        if (barrier->action) {
            barrier->action(barrier->arg);
        }
        err = reset_barrier(barrier, false);
        err = pthread_mutex_unlock(&barrier->lock);
        return index;
    }

    while (true) {
        err = pthread_cond_wait(&barrier->cond, &barrier->lock);
        if (barrier->broken) {
            err = pthread_mutex_unlock(&barrier->lock);
            return -1;
        }
        if (barrier->number_waiting == barrier->parties) {
            reset_barrier(barrier, true);
            err = pthread_mutex_unlock(&barrier->lock);
            return -1;
        }
        if (curr_gen != barrier->generations) {
            err = pthread_mutex_unlock(&barrier->lock);
            return  index;
        }
    }
}

void BarrierReset(CyclicBarrier* barrier) {
    assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    err = reset_barrier(barrier, false);
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
    return barrier->parties;// no lock needed unchanged value 
}

bool BarrierIsBroken(CyclicBarrier* barrier) {
    assert(barrier);
    int err;
    err = pthread_mutex_lock(&barrier->lock);
    bool  broken = barrier->broken;
    err = pthread_mutex_unlock(&barrier->lock);
    return broken;
}

