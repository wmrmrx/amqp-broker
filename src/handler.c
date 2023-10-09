#include <pthread.h>
#include <stdbool.h>
#include "handler.h"
#include "util.h"
#include "boilerplate.h"
#include "amqp_queue.h"

// Misc global variables
const ssize_t BUFFER_SIZE = 4096;

void* handle(void* _args) {
	struct args_t *args = _args;
	int connfd = args->connfd;
	ssize_t* num_queues = args->num_queues;
	pthread_mutex_t* num_queues_mutex = args->num_queues_mutex;
	struct amqp_queue* queues = args->queues;
	// buffer is used for everything so we don't need to worry about memory allocation
	char buffer[BUFFER_SIZE];

	// Check protocol header
	static const char PROTOCOL_HEADER[8] = {'A', 'M', 'Q', 'P', 0x00, 0x00, 0x09, 0x01};
	ok_read(connfd, buffer, 8);
	for(size_t i = 0; i < 8; i++) {
		if(buffer[i] != PROTOCOL_HEADER[i]) {
			// Header is invalid. Write back the correct header.
			ok_write(connfd, PROTOCOL_HEADER, 8);
			free(args); 
			return NULL;
		}
	}
	connection_start_boilerplate(buffer, connfd);
	struct frame_t frame = read_frame(buffer, connfd);
	ok_read(connfd, buffer, frame.size + 1);

	if(buffer[0] == 0x00 && buffer[1] == 0x32 && // QUEUE
		buffer[2] == 0x00 && buffer[3] == 0x0a // DECLARE
	) {
		pthread_mutex_lock(num_queues_mutex);

		ssize_t n = *num_queues;
		ssize_t queue_name_len = frame.size - 12;
		char* name = &buffer[7];
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
		}
		
		pthread_mutex_unlock(num_queues_mutex);

		// Cop out and just close the connection for simplicity
		close(connfd);
		free(args);
		return NULL;
	} else if(buffer[0] == 0x00 && buffer[1] == 0x3c && // BASIC
			buffer[2] == 0x00 && buffer[3] == 0x28 // PUBLISH
	) {
		ssize_t queue_name_len = frame.size - 9;
		char* name = &buffer[8];
		name[queue_name_len] = '\0';

		pthread_mutex_lock(num_queues_mutex);
		ssize_t n = *num_queues;
		pthread_mutex_unlock(num_queues_mutex);
		ssize_t queue_id;
		for(queue_id = 0; queue_id < n; queue_id++) {
			if(strcmp(name, queues[queue_id].name) == 0)
				break;
		}
		if(queue_id == n) { // queue with that name not found
			// Cop out and just close the connection for simplicity
			close(connfd);
			return NULL;
		}
		struct amqp_queue* queue = &queues[queue_id];

		frame = read_frame(buffer, connfd);
		ok_read(connfd, buffer, frame.size + 1); // Ignoring the Content Header

		frame = read_frame(buffer, connfd);
		ok_read(connfd, buffer, frame.size + 1); // Ignoring the Content Header
		char* message = malloc(frame.size);
		memcpy(message, buffer, frame.size);

		// message is moved
		publish_message(queue, message);

		// Cop out and just close the connection for simplicity
		close(connfd);
		free(args);
		return NULL;
	} else if(buffer[0] == 0x00 && buffer[1] == 0x3c && // BASIC
			buffer[2] == 0x00 && buffer[3] == 0x14 // CONSUME
	){ 
		ssize_t queue_name_len = frame.size - 13;
		char *name = &buffer[7];
		name[queue_name_len] = '\0';

		pthread_mutex_lock(num_queues_mutex);
		ssize_t n = *num_queues;
		pthread_mutex_unlock(num_queues_mutex);
		ssize_t queue_id;
		for(queue_id = 0; queue_id < n; queue_id++) {
			if(strcmp(name, queues[queue_id].name) == 0)
				break;
		}
		if(queue_id == n) { // queue with that name not found
			// Cop out and just close the connection for simplicity
			close(connfd);
			return NULL;
		}
		struct amqp_queue* queue = &queues[queue_id];

		// Just write the same consumer_tag for simplicity
		static const char* BASIC_CONSUME_OK = 
"\x01\x00\x01\x00\x00\x00\x24\x00\x3c\x00\x15\x1f\x61\x6d\x71\x2e" \
"\x63\x74\x61\x67\x2d\x5f\x62\x4c\x75\x56\x79\x32\x4f\x79\x61\x6c" \
"\x6f\x4f\x45\x31\x33\x71\x71\x34\x47\x41\x67\xce";
		ok_write(connfd, BASIC_CONSUME_OK, 44);

		// This connection will be managed by the distributor thread
		subscribe(queue, connfd);
		free(args);
		return NULL;
	} else {
	    perror("Request desconhecido :( \n");
	}

	// For simplicity, we are just ending the connection by closing the file descriptor, so this isn't needed
	// connection_end_boilerplate(buffer, connfd);
	free(args);
	return NULL;
}
