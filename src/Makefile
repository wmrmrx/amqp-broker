CC = gcc
# CFLAGS = -Wall -Wextra -Wshadow -pedantic -g -D_GLIBC_DEBUG -D_GLIBC_DEBUGPEDANTIC -fsanitize=address,undefined
CFLAGS = -Wall -Wextra -Wshadow -pedantic -g

SRC_FILES = $(wildcard *.c)
OBJ_FILES = $(SRC_FILES:.c=.o)

TARGET = broker

$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -f $(OBJ_FILES) $(TARGET)
