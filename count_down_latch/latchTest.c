#include "count_down_latch.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

#define THREAD_COUNT 10

typedef struct thread_arg {

	pthread_mutex_t* lock;
	pthread_cond_t* cond;

	CountDownLatch* latch;
	int* flag;
	int index;
	pthread_t id;
} thread_arg;

void* thread_fn(void* arg) {
	thread_arg* args = (thread_arg*)arg;
	while (1) {
		pthread_mutex_lock(args->lock);
		pthread_cond_wait(args->cond, args->lock);
		pthread_mutex_unlock(args->lock);
		//usleep(1000);
		printf("thread %d before \n", args->index);
		LatchCountDown(args->latch);
		//usleep(1000);
		printf("thread %d after \n", args->index);
		if (!(*args->flag))break;
	}
	printf("thread %d FINISHED\n", args->index);
	return NULL;
}


int main() {
	int flag = 1;
	thread_arg args[THREAD_COUNT];
	CountDownLatch latch;
	LatchInit(&latch, THREAD_COUNT);
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

	for (int i = 0; i < THREAD_COUNT; i++) {
		args[i].index = i;
		args[i].lock = &lock;
		args[i].cond = &cond;
		args[i].latch = &latch;
		args[i].flag = &flag;

		pthread_create(&args[i].id, NULL, thread_fn, &args[i]);
	}

	usleep(10 * 1000);
	flag = 0;
	pthread_cond_broadcast(&cond);
	LatchAwait(&latch);
	printf("all done\n");
	LatchDispose(&latch);
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&cond);
	for (int i = 0; i < THREAD_COUNT; i++) {
		pthread_join(args[i].id, NULL);
	}
	return 0;
}