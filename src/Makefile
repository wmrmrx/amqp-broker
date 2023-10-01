CC = gcc
CFLAGS = -Wall -g

build: main

main: main.o handler.o
	$(CC) $(CFLAGS) -o broker main.o handler.o

main.o: main.c handler.h
	$(CC) $(CFLAGS) -c main.c

handler.o: handler.c handler.h
	$(CC) $(CFLAGS) -c handler.c

clean:
	rm -f broker *.o
