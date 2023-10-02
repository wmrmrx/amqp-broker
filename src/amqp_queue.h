#ifndef _AMQP_QUEUE_H
#define _AMQP_QUEUE_H

#include <pthread.h>
#include <stdbool.h>

struct message_node {
	char* message;
	struct message_node *prev, *next;
};

struct subscriber_node {
	int connfd;
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

void initialize_amqp_queue(struct amqp_queue* queue, char* name);
void publish_message(struct amqp_queue* queue, char* message);
void distribute_messages(struct amqp_queue* queue);
void subscrite(struct amqp_queue* queue, int connfd);
void unsubscrite(struct amqp_queue* queue, int connfd);

#endif
