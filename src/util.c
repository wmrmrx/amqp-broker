#include <assert.h>
#include <unistd.h>
#include "util.h"
#include <fcntl.h>


int try_write(int fd, const unsigned char* buffer, size_t size) {
    if (fd < 0 || fcntl(fd, F_GETFD) == -1)
        return -1;
    if (write(fd, buffer, size) != (ssize_t) size)
        return -1;
    return 0;
}

void ok_write(int fd, const unsigned char* buffer, size_t size) {
	assert(try_write(fd, buffer, size) != -1);
}

int try_read(int fd, unsigned char* buffer, size_t size) {
    if (fd < 0 || fcntl(fd, F_GETFD) == -1)
        return -1;
    if (read(fd, buffer, size) != (ssize_t) size)
        return -1;
    return 0;
}

void ok_read(int fd, unsigned char* buffer, size_t size) {
	assert(try_read(fd, buffer, size) != -1);
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
