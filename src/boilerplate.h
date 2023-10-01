#ifndef _BOILERPLATE_H
#define _BOILERPLATE_H

// Boilerplate to handle the start and end of a AQMP connection. Copied from RabbitMQ for the sake of simplicity.
void connection_start_boilerplate(char* buffer, int connfd);
void connection_end_boilerplate(char* buffer, int connfd);

#endif
