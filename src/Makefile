CC = gcc
CFLAGS = -Wall -Wextra -Wshadow -pedantic -g -D_GLIBC_DEBUG -D_GLIBC_DEBUGPEDANTIC

build: main

main: main.o handler.o boilerplate.o util.o amqp_queue.o
	$(CC) $(CFLAGS) -o broker main.o handler.o boilerplate.o util.o

main.o: main.c handler.h
	$(CC) $(CFLAGS) -c main.c

handler.o: handler.c handler.h
	$(CC) $(CFLAGS) -c handler.c

boilerplate.o: boilerplate.c boilerplate.h
	$(CC) $(CFLAGS) -c boilerplate.c

util.o: util.c util.h
	$(CC) $(CFLAGS) -c util.c

amqp_queue.o: amqp_queue.c amqp_queue.h
	$(CC) $(CFLAGS) -c amqp_queue.c

clean:
	rm -f broker *.o
