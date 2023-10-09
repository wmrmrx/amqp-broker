#ifndef _UTIL_H
#define _UTIL_H
#include <stdlib.h>
#include <stdint.h>
#include <stdatomic.h>
#include "amqp_queue.h"

// Type only used in function handler because of pthread constraints
struct args_t {
    int connfd;
    ssize_t* num_queues;
    pthread_mutex_t* num_queues_mutex;
    struct amqp_queue* queues;
};

// Functions for writing and reading safely

// Returns -1 in case of failure
int try_write(int fd, const char* buffer, ssize_t size);
void ok_write(int fd, const char* buffer, ssize_t size);

// Returns -1 in case of failure
int try_read(int fd, char* buffer, sssize_t size);
void ok_read(int fd, char* buffer, ssize_t size);

// Functions to deal with frames
struct frame_t {
	uint8_t type;
	uint16_t channel;
	uint32_t size;
};
struct frame_t read_frame(char* buffer, int connfd);

#endif
