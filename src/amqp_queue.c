#include "amqp_queue.h"
#include "util.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

void initialize_amqp_queue(struct amqp_queue* queue, char* name) {
	strcpy(queue->name, name);
	queue->message_queue_head = NULL;
	queue->subscriber_node_head = NULL;
	pthread_mutex_init(&queue->mutex, NULL);
}

void publish_message(struct amqp_queue* queue, unsigned char* message, size_t message_len) {
	pthread_mutex_lock(&queue->mutex);
	struct message_node* new_head = malloc(sizeof(struct message_node));
	new_head->message = message;
	new_head->message_len = message_len;
	new_head->next = queue->message_queue_head;
	queue->message_queue_head = new_head;
	pthread_mutex_unlock(&queue->mutex);
}

void unsubscribe(struct subscriber_node** node_ptr) {
	struct subscriber_node* node = *node_ptr;

	// Only one subscriber
	if(node->prev == node) {
		*node_ptr = NULL;
	} else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
		*node_ptr = node->next;
	}

	close(node->connfd);
	free(node);
}

// Returns -1 if there's no subscribers or no messages
int round_robin(struct message_node* msg_node, struct subscriber_node** head) {
	if(*head == NULL || msg_node == NULL) 
		return -1;
	assert(msg_node);
	if(msg_node->next)
		if(round_robin(msg_node->next, head) == -1)
			return -1;
	size_t msg_len = msg_node->message_len;


	static unsigned char buffer[4096]; // Ok to do this because only one thread is in charge of distributing messages
	bool err = false;
	do {
		static const unsigned char* BASIC_DELIVER = (unsigned char*)
	"\x01\x00\x01\x00\x00\x00\x31\x00\x3c\x00\x3c\x1f\x61\x6d\x71\x2e" \
	"\x63\x74\x61\x67\x2d\x5f\x62\x4c\x75\x56\x79\x32\x4f\x79\x61\x6c" \
	"\x6f\x4f\x45\x31\x33\x71\x71\x34\x47\x41\x67\x00\x00\x00\x00\x00" \
	"\x00\x00\x01\x00\x00\x02\x71\x31\xce";

		// Now write message to subscriber
		if( try_write((*head)->connfd, BASIC_DELIVER, 57) == -1 ) {
			err = true;
			break;
		}

		static const unsigned char* CONTENT_HEADER = (unsigned char*)
	"\x02\x00\x01\x00\x00\x00\x0f\x00\x3c\x00\x00\xff\xff\xff\xff\xff" \
	"\xff\xff\xff\x10\x00\x01\xce";
		memcpy(buffer, CONTENT_HEADER, 23);
		// write body size in big endian
		buffer[11] = (unsigned char) (msg_len >> 56);
		buffer[12] = (unsigned char) (msg_len >> 48);
		buffer[13] = (unsigned char) (msg_len >> 40);
		buffer[14] = (unsigned char) (msg_len >> 32);
		buffer[15] = (unsigned char) (msg_len >> 24);
		buffer[16] = (unsigned char) (msg_len >> 16);
		buffer[17] = (unsigned char) (msg_len >> 8 );
		buffer[18] = (unsigned char) (msg_len      );
		if( try_write((*head)->connfd, buffer, 23) == -1 ) {
			err = true;
			break;
		}

		// write CONTENT_BODY
		buffer[0] = 0x03;
		buffer[1] = 0x00;
		buffer[2] = 0x01;
		buffer[3] = (unsigned char) (msg_len >> 24);
		buffer[4] = (unsigned char) (msg_len >> 16);
		buffer[5] = (unsigned char) (msg_len >> 8 );
		buffer[6] = (unsigned char) (msg_len      );
		memcpy(buffer + 7, msg_node->message, msg_len);
		buffer[7 + msg_len] = 0xce;
		if( try_write((*head)->connfd, buffer, msg_len + 8) == -1 ) {
			err = true;
			break;
		}

		// Read Basic.ACK
		struct frame_t ret;
		if( try_read((*head)->connfd, buffer, 7) == -1 ) {
			err = true;
			break;
		}
		ret.type = (uint8_t) buffer[0];
		ret.channel = (((uint16_t) buffer[1]) << 8) +
			      (((uint16_t) buffer[2]));
		ret.size = (((uint32_t) buffer[3]) << 24) +
			   (((uint32_t) buffer[4]) << 16) +
			   (((uint32_t) buffer[5]) << 8)  +
			   (((uint32_t) buffer[6]));
		if( try_read((*head)->connfd, buffer, ret.size + 1) == -1 ) {
			err = true; 
			break;
		}

	} while(false);


	// Read the Basic.ACK, if there isn't unsubscribe the current node
	if(err) {
		unsubscribe(head);
		if(*head == NULL) 
			return -1;
	}

	free(msg_node->message);
	free(msg_node);
	*head = (*head)->next;

	return 0;
}

void distribute_messages(struct amqp_queue* queue) {
	pthread_mutex_lock(&queue->mutex);
	if( round_robin(queue->message_queue_head, &queue->subscriber_node_head) != -1 )
		queue->message_queue_head = NULL;
	pthread_mutex_unlock(&queue->mutex);
}

void subscribe(struct amqp_queue* queue, int connfd) {
	pthread_mutex_lock(&queue->mutex);
	struct subscriber_node* new_node = malloc(sizeof(struct subscriber_node));
	new_node->connfd = connfd;

	if(queue->subscriber_node_head) {
		new_node->prev = queue->subscriber_node_head->prev;
		new_node->next = queue->subscriber_node_head;
		new_node->prev->next = new_node;
		new_node->next->prev = new_node;
	} else {
		new_node->prev = new_node->next = new_node;
		queue->subscriber_node_head = new_node;
	}

	pthread_mutex_unlock(&queue->mutex);
}

