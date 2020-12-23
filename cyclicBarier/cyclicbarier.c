#include "cyclicBarier.h"
#include <pthread.h>
#include <semaphore.h>

typedef struct  {
    int count;
    pthread_mutex_t lock;
    pthread_cond_t cond;
    sem_t sem;
} CyclicBarier;

void init(CyclicBarier* barier, int count) {
    assert(barier);
    assert(count>0);
    barier->count = count;
    pthread_mutex_init(&barier->lock,NULL);
    pthread_cond_init(&barier->cond,NULL);
    sem_init(&barier->sem,0,0);
}

void LatchAwait(CyclicBarier* barier) {
     barier->lock;
     sem_post(&barier->sem);
}
getNumberWaiting() {

}
getParties() {

}
isBroken() {

}
reset() {
    
}