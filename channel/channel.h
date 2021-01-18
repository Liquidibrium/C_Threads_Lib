#ifndef __channel_
#define __channel_

#include "./array_queue/array_queue.h"
#include "pthread.h"


typedef struct channel {
    ArrayQueue que;         // channel messages stored there 
    bool running;           // indicates if chan is not closed

    pthread_mutex_t lock;

    pthread_cond_t receive;
    pthread_cond_t send;

    size_t count_sending;   // registered senders 
    size_t count_receiving; // registered receivers 
} chan;

/**
 * @brief  initializes buffered channel with capacity, elements size and obligatory not null free function
 * @param ch
 * @param capacity
 * @param elem_size
 * @param free_fn
 * @return int  error code
 */
int ChannelInit(chan* ch, size_t capacity, size_t elem_size, void (*free_fn)(void*));

/**
 * @brief cleans memory obtained by chan
 * @param ch
 * @return int
 */
int ChannelDispose(chan* ch);

/**
 * @brief sends message pointer in channel
 * @param ch
 * @param message_to_send_ptr
 * @return int
 */
int ChannelSend(chan* ch, void* message_to_send_ptr);

/**
 * @brief receives message from channel as pointer
 *
 * @param ch
 * @param message_to_receive_ptr
 * @return int
 */
int ChannelReceive(chan* ch, void* message_to_receive_ptr);

/**
 *  first part of @param channels is receiver channels (to which message can be send) then senders (from which message can be received)
 *  same order is in @param messages, at first messages to be send, then messages to be received
 *  their numbers are passed as @param num_receivers and  @param num_senders parameters
 *
 *  this method chooses available channels to do operation and
 *  if there is some of them chooses randomly one from them
 *
 *  this method is preferred to be used with switch statement, because
 *  @return int is index of channel
 *  returns -1 if no channel is available
 */
int ChannelSelect(chan* channels[], void* messages[], int num_receivers, int num_senders);

/**
 * @brief makes running false,
 *        releases threads waiting on condition,
 *        and dont allows other operations
 * @param ch
 * @return int
 */
int ChannelClose(chan* ch);


#endif