#include "future.h"
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>
void * Double(void * arg){
    int x = (int) arg;
    sleep(1);
    return  (void *) (2 * x) ;
}
void* square(void * arg){
    int* x =  (int*) arg;
    sleep(10);
    int * square = malloc(sizeof(int));
    *square = (*x) * (*x);
    return  square;
}

void FutureTest1(){
    Future fut;
    int x = 2;
    FutureInit(&fut,Double,(void*)x);
    int res;
    assert(FutureGetResult(&fut,(void**)&res)== 0);
    assert(res==4);
    FutureDispose(&fut);
    printf("Test 1 success\n");
}

void FutureTest2(){
    Future fut;
    int x = 2;
    FutureInit(&fut,Double,(void*)x);
    int res;
    FutureCancel(&fut);
    assert(FutureIsCancelled(&fut)==1);
    assert(FutureGetResult(&fut,(void**)&res)==-1);
    assert(FutureIsDone(&fut)==0);
    FutureDispose(&fut);
    printf("Test 2 success\n");
}
void FutureTest3(){
    Future fut;
    int x = 5;
    FutureInit(&fut,square,&x);
    void* res;
    FutureGetResult(&fut, &res);
    assert(*(int*)res==25);
    free(res);
    FutureDispose(&fut);
    printf("Test 3 success\n");
}

void FutureTest4(){
    Future fut;
    int x = 5;
    FutureInit(&fut,square,&x);
    void* res;
    long time = 1; // less than 10 
    assert(FutureTimedGetResult(&fut, &res, time) == ETIMEDOUT);
    FutureDispose(&fut);
    printf("Test 4 success\n");
}
int main(){
    FutureTest1();
    FutureTest2();
    FutureTest3();
    FutureTest4();
    return 0;
}