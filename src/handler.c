#include "handler.h"

void handle(int connfd) {
	char recvline[MAXLINE + 1];
	ssize_t n;
	while ((n=read(connfd, recvline, MAXLINE)) > 0) {
		recvline[n]=0;
		printf("[Cliente conectado no processo filho %d enviou:] ",getpid());
		if ((fputs(recvline,stdout)) == EOF) {
			perror("fputs :( \n");
			exit(6);
		}
		write(connfd, recvline, strlen(recvline));
	}
}
