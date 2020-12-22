#ifndef __channel_
#define __channel_

#include "./array_queue/array_queue.h"
#include "pthread.h"


typedef struct channel {
    ArrayQueue que;

    bool running;

    pthread_mutex_t lock;

    pthread_cond_t receive;
    pthread_cond_t send;

    size_t count_sending;
    size_t count_receiving;

} chan;

int ChannelInit(chan * ch, size_t capacity, size_t elem_size,void (*free_fn)(void *));

int ChannelDispose(chan* ch);

int ChannelSend(chan *ch, void *message_to_send);

int ChannelReceive(chan *ch, void *message_to_receive);

int ChannelSelect(chan *channels[], void * messages[], int num_receivers, int num_senders);

int ChannelClose(chan *ch);

 
#endif