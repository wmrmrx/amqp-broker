#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>

// Message Linked List
struct message_node {
	char* message;
	struct message_node *prev, *next;
};

typedef void(*subscriber_callback)(int, char*);
struct subscriber_node {
	subscriber_callback callback;
	struct subscriber_node *prev, *next;
};

// Defines a amqp queue type
struct amqp_queue {
	char name[256];

	// Consumed from head, published to last
	struct message_node* message_queue_head;
	struct message_node* message_queue_last;

	struct subscriber_node* subscriber_node_head;

	pthread_mutex_t mutex;
};

#endif
