#ifndef _AMQP_QUEUE_H
#define _AMQP_QUEUE_H

#include <pthread.h>
#include <stdbool.h>

struct message_node {
	unsigned char* message;
	size_t message_len;
	struct message_node *next;
};

struct subscriber_node {
	int connfd;
	struct subscriber_node *prev, *next;
};

// Defines a amqp queue type
struct amqp_queue {
	char name[256];

	// Singly Linked List
	struct message_node* message_queue_head;
	// Circular Linked List
	struct subscriber_node* subscriber_node_head;

	pthread_mutex_t mutex;
};

// Self-documenting names
void initialize_amqp_queue(struct amqp_queue* queue, char* name);
void publish_message(struct amqp_queue* queue, unsigned char* message, size_t message_len);
void distribute_messages(struct amqp_queue* queue);
void subscribe(struct amqp_queue* queue, int connfd);

#endif
