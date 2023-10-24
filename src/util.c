#include <assert.h>
#include <unistd.h>
#include "util.h"

int try_write(int fd, const unsigned char* buffer, ssize_t size) {
	assert( write(fd, buffer, size) == size );
}

void ok_write(int fd, const unsigned char* buffer, ssize_t size) {
	assert( write(fd, buffer, size) == size );
}

int try_read(int fd, unsigned char* buffer, ssize_t size) {
	assert( read(fd, buffer, size) == size );
}

void ok_read(int fd, unsigned char* buffer, ssize_t size) {
	assert( read(fd, buffer, size) == size );
}

struct frame_t read_frame(unsigned char* buffer, int connfd) {
	struct frame_t ret;

	ok_read(connfd, buffer, 7);

	ret.type = (uint8_t) buffer[0];
	ret.channel = (((uint16_t) buffer[1]) << 8) +
		      (((uint16_t) buffer[2]));
	ret.size = (((uint32_t) buffer[3]) << 24) +
		   (((uint32_t) buffer[4]) << 16) +
		   (((uint32_t) buffer[5]) << 8)  +
	           (((uint32_t) buffer[6]));

	return ret;
}
