#include <assert.h>
#include <unistd.h>
#include "util.h"

void okwrite(int fd, const char* buffer, ssize_t size) {
	assert( write(fd, buffer, size) == size );
}

void okread(int fd, char* buffer, ssize_t size) {
	assert( read(fd, buffer, size) == size );
}

struct frame_t read_frame(char* buffer, int connfd) {
	struct frame_t ret;

	okread(connfd, buffer, 7);

	ret.type = (uint8_t) buffer[0];
	ret.channel = (((uint16_t) buffer[1]) << 8) +
		      (((uint16_t) buffer[2]));
	ret.size = (((uint32_t) buffer[3]) << 24) +
		   (((uint32_t) buffer[4]) << 16) +
		   (((uint32_t) buffer[5]) << 8)  +
	           (((uint32_t) buffer[6]));

	return ret;
}
