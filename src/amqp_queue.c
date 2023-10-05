#include "amqp_queue.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void initialize_amqp_queue(struct amqp_queue* queue, char* name) {
	strcpy(queue->name, name);
	queue->message_queue_head = NULL;
	queue->subscriber_node_head = NULL;
	pthread_mutex_init(&queue->mutex, NULL);
}

void publish_message(struct amqp_queue* queue, char* message) {
	pthread_mutex_lock(&queue->mutex);
	struct message_node* new_head = malloc(sizeof(struct message_node));
	new_head->message = message;
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

// Returns -1 if there's no subscribers
int round_robin(struct message_node* msg_node, struct subscriber_node** head) {
	if(*head == NULL) 
		return -1;
	if(msg_node->next)
		if(round_robin(msg_node->next, head) == -1)
			return -1;
	ssize_t msg_len = strlen(msg_node->message);

	static char buffer[4096]; // Ok to do this because only one thread is in charge of distributing messages

	// Now write message to subscriber
	// TODO
	if( write((*head)->connfd, msg_node->message, msg_len) != msg_len )
		return -1;

	// Read the Basic.ACK, if there isn't unsubscribe the current node
	{
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
	round_robin(queue->message_queue_head, &queue->subscriber_node_head);
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

