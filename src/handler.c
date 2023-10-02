#include "handler.h"
#include "util.h"
#include "boilerplate.h"
#include "amqp_queue.h"

// Misc global variables
const ssize_t BUFFER_SIZE = 4096;

void* handle(void* args) {
	int connfd = ((struct args_t*)args)->connfd;
	atomic_size_t* num_queues = ((struct args_t*) args)->num_queues;
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

	if(buffer[0] == 0x00 && buffer[1] == 0x32 // QUEUE
		buffer[2] == 0x00 && buffer[3] == 0x0a // DECLARE
	) {
	}

	connection_end_boilerplate(buffer, connfd);
	return 0;
}
