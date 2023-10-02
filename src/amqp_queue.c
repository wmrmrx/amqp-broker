#include "amqp_queue.h"

// Message Linked List
void initialize_amqp_queue(struct amqp_queue* queue, char* name);
void publish_message(struct amqp_queue* queue, char* message);
void distribute_messages(struct amqp_queue* queue);
void subscribe(struct amqp_queue* queue, int connfd);
void unsubscribe(struct amqp_queue* queue, int connfd);
