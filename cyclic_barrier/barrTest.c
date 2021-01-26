#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include "cyclic_barrier.h"
#include <assert.h>

void* barrier_test(void* args) {
    int parties = BarrierGetParties(args);
    int waiting = BarrierGetNumberWaiting(args);
    int index = BarrierAwait(args);

    //printf("\tawait %d %d %d\n",index, waiting,parties);
    assert(parties - index - 1 == waiting);
    return  NULL;
}

void cout() {
    printf("\naction fn performed\n");
}

void test1() {
    CyclicBarrier barr;
    BarrierInit(&barr, 10, cout, NULL);
    pthread_t tid[10];
    int thread_count = 10;
    for (int i = 0; i < 10; i++) {
        pthread_create(&(tid[i]), NULL, &barrier_test, &barr);
    }
    for (int j = 0; j < 10; j++) {
        pthread_join(tid[j], NULL);
    }

    BarrierDispose(&barr);
    printf("test 1: success \n");
}



int main() {

    test1();
    return 0;
}