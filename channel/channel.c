#include "channel.h"
#include <stdbool.h>
#include <time.h>

int ChannelInit(chan *ch, size_t capacity, size_t elem_size, void (*free_fn)(void *))
{
    assert(ch);
    assert(capacity > 0);
    assert(elem_size > 0);
    assert(free_fn);
    int err;
    ArrayQueueInit(&ch->que, capacity, elem_size, free_fn);
    ch->running = true;
    // ch->capacity = capacity;
    // ch->elem_size = elem_size;
    // ch->count = 0;
    ch->count_sending = 0;
    ch->count_receiving = 0;
    err = pthread_cond_init(&ch->receive, NULL);
    err = pthread_cond_init(&ch->send, NULL);
    err = pthread_mutex_init(&ch->lock, NULL);
    return err;
}

int ChannelDispose(chan *ch)
{
    int err;
    err = ArrayQueueDispose(&ch->que);
    err = pthread_mutex_destroy(&ch->lock);
    err = pthread_cond_destroy(&ch->receive);
    err = pthread_cond_destroy(&ch->send);
    return err;
}

int ChannelSend(chan *ch, void *message_to_send)
{
    assert(ch);
    assert(message_to_send);
    int err;
    err = pthread_mutex_lock(&ch->lock);
    while (ArrayQueueIsFull(&ch->que))
    {                     // wait for enough space to add
        if (!ch->running) // channel is closed
        {
            err = pthread_mutex_unlock(&ch->lock);
            return -1;
        }
        ch->count_sending++;
        err = pthread_cond_wait(&ch->send, &ch->lock);
        ch->count_sending--;
    }

    err = ArrayQueuePushBack(&ch->que, message_to_send);

    if (ch->count_receiving > 0)
    { // signal receiver to get message
        err = pthread_cond_signal(&ch->receive);
    }

    err = pthread_mutex_unlock(&ch->lock);
    return err;
}

int ChannelReceive(chan *ch, void *message_to_receive)
{
    assert(ch);
    assert(message_to_receive);
    int err = pthread_mutex_lock(&ch->lock);
    while (ArrayQueueIsEmpty(&ch->que))
    {                     // Nothing to get from channel
        if (!ch->running) // channel is closed
        {
            err = pthread_mutex_unlock(&ch->lock);
            return -1;
        }
        ch->count_receiving++;
        err = pthread_cond_wait(&ch->receive, &ch->lock);
        ch->count_receiving--;
    }

    err = ArrayQueueGetFront(&ch->que, message_to_receive);

    if (ch->count_sending > 0)
    { // signal sender add message
        err = pthread_cond_signal(&ch->send);
    }

    err = pthread_mutex_unlock(&ch->lock);
    return err;
}

/*
    chan* channels[], containts first receivers (to which message can be send) then senders (from which message can be received) 
    same order is in messages, at first messages to be send, then messages to be receivede 
    their numbers are passed as num_receivers and num_senders parameters 

    this method chooses avaiable channels to do operation and 
    if there is some of them chooses randomly one from them

    returns index of channel so 
    this method preffers to be used with switch statement

    returns -1 if no channel is available
*/
int ChannelSelect(chan *channels[], void *messages[], int num_receivers, int num_senders)
{
    int count_available_channels = 0;
    int number_of_channels = num_receivers + num_senders;
    int indexes[number_of_channels]; // contains indexes of valid channels to do operation
    for (int i = 0; i < number_of_channels; i++)
    {
        bool isAvailable = false;
        if (i < num_receivers)
        { // if can receive
            pthread_mutex_lock(&channels[i]->lock);
            isAvailable == !ArrayQueueIsFull(&channels[i]->que);
            pthread_mutex_unlock(&channels[i]->lock);
        }
        else
        { // if can send
            pthread_mutex_lock(&channels[i]->lock);
            isAvailable == !ArrayQueueIsEmpty(&channels[i]->que);
            pthread_mutex_unlock(&channels[i]->lock);
        }
        if (isAvailable)
        {
            indexes[count_available_channels] = i;
            count_available_channels++;
        }
    }
    if (count_available_channels == 0)
        return -1; // no availabele channels

    srand(time(0)); // if there is some available channels we should choose randomly
    int selected_channel = indexes[rand() % count_available_channels];
    if (selected_channel < num_receivers)
    { // if channel is receiver we should send message
        if (ChannelSend(channels[selected_channel], messages[selected_channel]) == -1)
            return -1;
    }
    else
    { // channel is sender so we should receive message
        if (ChannelReceive(channels[selected_channel], messages[selected_channel]) == -1)
            return -1;
    }
    return selected_channel;
}

int ChannelClose(chan *ch)
{
    int err = pthread_mutex_lock(&ch->lock);
    if (!ch->running)
    { // already closed
        err = pthread_mutex_unlock(&ch->lock);
        return -1;
    }
    ch->running = false;
    err = pthread_cond_broadcast(&ch->send);
    err = pthread_cond_broadcast(&ch->receive);
    err = pthread_mutex_unlock(&ch->lock);
    return 0;
}