#include <pthread.h>
#include <stdbool.h>
#include "handler.h"
#include "util.h"
#include "boilerplate.h"
#include "amqp_queue.h"

// Misc global variables
const ssize_t BUFFER_SIZE = 4096;

void* handle(void* args) {
	int connfd = ((struct args_t*)args)->connfd;
	ssize_t* num_queues = ((struct args_t*) args)->num_queues;
	pthread_mutex_t* num_queues_mutex = ((struct args_t*) args)->num_queues_mutex;
	struct amqp_queue* queues = ((struct args_t*) args)->queues;
	// buffer is used for everything so we don't need to worry about memory allocation
	char buffer[BUFFER_SIZE];

	// Check protocol header
	static const char PROTOCOL_HEADER[8] = {'A', 'M', 'Q', 'P', 0x00, 0x00, 0x09, 0x01};
	okread(connfd, buffer, 8);
	for(size_t i = 0; i < 8; i++) {
		if(buffer[i] != PROTOCOL_HEADER[i]) {
			// Header is invalid. Write back the correct header.
			okwrite(connfd, PROTOCOL_HEADER, 8);
			pthread_exit(NULL);
		}
	}
	connection_start_boilerplate(buffer, connfd);
	struct frame_t frame = read_frame(buffer, connfd);
	okread(connfd, buffer, frame.size + 1);

	if(buffer[0] == 0x00 && buffer[1] == 0x32 && // QUEUE
		buffer[2] == 0x00 && buffer[3] == 0x0a // DECLARE
	) {
		pthread_mutex_lock(num_queues_mutex);
		ssize_t n = *num_queues;
		ssize_t queue_name_len = frame.size - 20;
		char* name = &buffer[13];
		name[queue_name_len] = '\0';

		// Check if there's already a queue with that name
		bool found_queue = false;
		for(ssize_t i=0;i<n;i++) if(strcmp(name, queues[i].name) == 0) {
			found_queue = true;
			break;
		}
		if(!found_queue) {
			initialize_amqp_queue(&queues[n], name);
			(*num_queues)++;
		
		pthread_mutex_unlock(num_queues_mutex);

		// Cop out and just close the connection for simplicity
		close(connfd);
		return NULL;
	}

	connection_end_boilerplate(buffer, connfd);
	return NULL;
}
