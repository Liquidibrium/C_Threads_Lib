#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include "channel.h"

const char* const kQuestionWords[] = { "who0", "what1", "where2", "how3", "why4" };
const int kNumQuestionWords = 5;

static void FreeString(void* elemAddr) {
    char* s = *(char**)elemAddr;
    free(s);
}
int threadNUM = 3;
int count = 15;
void* receiver(void* args) {
    chan* ch = (chan*)args;
    while (true) {
        char* str;
        ChannelReceive(ch, &str);
        count--;
        // printf("\t%s\n", str);
        free(str);
        if (count == 0) {
            break;
        }
    }
    return NULL;
}
void* senders(void* args) {
    for (int i = 0; i < kNumQuestionWords; i++) {
        char* send = strdup(kQuestionWords[i]);
        ChannelSend((chan*)args, &send);
    }
    return NULL;
}
void testStr() {
    chan ch;
    ChannelInit(&ch, 1, sizeof(char*), FreeString);
    pthread_t sender[threadNUM];
    for (size_t i = 0; i < threadNUM; i++) {
        pthread_create(sender + i, NULL, senders, &ch);
    }

    pthread_t thr;
    pthread_create(&thr, NULL, receiver, &ch);

    for (size_t i = 0; i < threadNUM; i++) {
        pthread_join(sender[i], NULL);
    }
    pthread_join(thr, NULL);
    ChannelDispose(&ch);
    printf("success string\n");
}
//_________________________________________________________________________
pthread_mutex_t lock;
int counter = 0;
void* sender1(void* args) {
    int x = 123;
    pthread_mutex_lock(&lock);
    counter++;
    pthread_mutex_unlock(&lock);
    ChannelSend((chan*)args, &counter);
    return NULL;
}

void* receiver1(void* args) {
    int x = 0;
    ChannelReceive((chan*)args, &x);
    pthread_mutex_lock(&lock);
    counter--;
    pthread_mutex_unlock(&lock);
    // printf("%d\n", x);
    return NULL;
}

void testWaiting() {
    pthread_mutex_init(&lock, NULL);
    chan ch;
    ChannelInit(&ch, 5, sizeof(int), NULL);
    pthread_t th[100];
    for (int i = 0; i < 50; ++i) {
        pthread_create(&th[i], NULL, sender1, &ch);
    }

    while (true) {
        pthread_mutex_lock(&ch.lock);
        bool all_waiting = ch.count_sending == 45;
        pthread_mutex_unlock(&ch.lock);
        if (all_waiting) break;
    }

    for (int i = 50; i < 100; ++i) {
        pthread_create(&th[i], NULL, receiver1, &ch);
    }
    for (int i = 0; i < 100; ++i) {
        pthread_join(th[i], NULL);
    }
    ChannelDispose(&ch);
    pthread_mutex_destroy(&lock);
    assert(counter == 0);
    printf("success waiting\n");
}

//_________________________________________________________________________

void test1() {
    chan ch;
    ChannelInit(&ch, 1, sizeof(double), NULL);
    double x = 1;
    ChannelSend(&ch, &x);
    double res = 0;
    ChannelReceive(&ch, &res);
    assert(res == 1);
    ChannelDispose(&ch);
    printf("success test 1 \n");
}

void  test2() {
    chan ch;
    ChannelInit(&ch, 1, sizeof(double*), free);
    double y = 2.6;
    double* x = malloc(sizeof(double));
    *x = y;
    ChannelSend(&ch, &x);
    double* res = 0;
    ChannelReceive(&ch, &res);
    //  printf("%f\n", *res);
    ChannelDispose(&ch);
    free(x);
    printf("success test 2 \n");
}

void testSelect() {
    chan* channels[2];
    for (int i = 0; i < 2; i++) {
        channels[i] = malloc(sizeof(chan));
        ChannelInit(channels[i], 1, sizeof(int), NULL);
    }
    int* messages[2];
    messages[0] = malloc(sizeof(int));
    messages[1] = malloc(sizeof(int));
    *messages[0] = 123;// mesage to send 
    *messages[1] = 5; // message to receive 

    int messageToSend = 199;
    ChannelSend(channels[1], &messageToSend);

    switch (ChannelSelect(channels, messages, 1, 1)) {// randomly choses
    case 0:
        printf("received \n");
        //printf("%d\n", *messages[0]);
        break;
    case 1:
        printf("sended %d\n", *messages[1]);
        break;
    default:
        printf("default \n");
        break;
    }
    for (int i = 0; i < 2; i++) {
        ChannelDispose(channels[i]);
        free(channels[i]);
        free(messages[i]);
    }
}

int main() {
    test1();
    test2();
    testStr();
    testWaiting();
    testSelect();
    return 0;
}



