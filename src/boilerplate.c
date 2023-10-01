#include <stdbool.h>
#include "boilerplate.h"
#include "util.h"

void connection_start_boilerplate(char* buffer, int connfd) {
	static const char CONNECTION_START[] = 
"\x02\x42\x86\xae\xff\x65\x02\x42\xac\x11\x00\x02\x08\x00\x45\x00" \
"\x02\x38\x54\xf1\x40\x00\x40\x06\x8b\xa9\xac\x11\x00\x02\xac\x11" \
"\x00\x01\x16\x28\xc7\x18\x88\xae\x8d\x4d\x9b\x75\x86\x91\x80\x18" \
"\x01\xfd\x5a\x50\x00\x00\x01\x01\x08\x0a\x24\x4b\xe1\x08\xaf\x03" \
"\x37\x6b\x01\x00\x00\x00\x00\x01\xfc\x00\x0a\x00\x0a\x00\x09\x00" \
"\x00\x01\xd7\x0c\x63\x61\x70\x61\x62\x69\x6c\x69\x74\x69\x65\x73" \
"\x46\x00\x00\x00\xc7\x12\x70\x75\x62\x6c\x69\x73\x68\x65\x72\x5f" \
"\x63\x6f\x6e\x66\x69\x72\x6d\x73\x74\x01\x1a\x65\x78\x63\x68\x61" \
"\x6e\x67\x65\x5f\x65\x78\x63\x68\x61\x6e\x67\x65\x5f\x62\x69\x6e" \
"\x64\x69\x6e\x67\x73\x74\x01\x0a\x62\x61\x73\x69\x63\x2e\x6e\x61" \
"\x63\x6b\x74\x01\x16\x63\x6f\x6e\x73\x75\x6d\x65\x72\x5f\x63\x61" \
"\x6e\x63\x65\x6c\x5f\x6e\x6f\x74\x69\x66\x79\x74\x01\x12\x63\x6f" \
"\x6e\x6e\x65\x63\x74\x69\x6f\x6e\x2e\x62\x6c\x6f\x63\x6b\x65\x64" \
"\x74\x01\x13\x63\x6f\x6e\x73\x75\x6d\x65\x72\x5f\x70\x72\x69\x6f" \
"\x72\x69\x74\x69\x65\x73\x74\x01\x1c\x61\x75\x74\x68\x65\x6e\x74" \
"\x69\x63\x61\x74\x69\x6f\x6e\x5f\x66\x61\x69\x6c\x75\x72\x65\x5f" \
"\x63\x6c\x6f\x73\x65\x74\x01\x10\x70\x65\x72\x5f\x63\x6f\x6e\x73" \
"\x75\x6d\x65\x72\x5f\x71\x6f\x73\x74\x01\x0f\x64\x69\x72\x65\x63" \
"\x74\x5f\x72\x65\x70\x6c\x79\x5f\x74\x6f\x74\x01\x0c\x63\x6c\x75" \
"\x73\x74\x65\x72\x5f\x6e\x61\x6d\x65\x53\x00\x00\x00\x13\x72\x61" \
"\x62\x62\x69\x74\x40\x64\x61\x66\x38\x33\x62\x32\x62\x30\x62\x36" \
"\x39\x09\x63\x6f\x70\x79\x72\x69\x67\x68\x74\x53\x00\x00\x00\x37" \
"\x43\x6f\x70\x79\x72\x69\x67\x68\x74\x20\x28\x63\x29\x20\x32\x30" \
"\x30\x37\x2d\x32\x30\x32\x33\x20\x56\x4d\x77\x61\x72\x65\x2c\x20" \
"\x49\x6e\x63\x2e\x20\x6f\x72\x20\x69\x74\x73\x20\x61\x66\x66\x69" \
"\x6c\x69\x61\x74\x65\x73\x2e\x0b\x69\x6e\x66\x6f\x72\x6d\x61\x74" \
"\x69\x6f\x6e\x53\x00\x00\x00\x39\x4c\x69\x63\x65\x6e\x73\x65\x64" \
"\x20\x75\x6e\x64\x65\x72\x20\x74\x68\x65\x20\x4d\x50\x4c\x20\x32" \
"\x2e\x30\x2e\x20\x57\x65\x62\x73\x69\x74\x65\x3a\x20\x68\x74\x74" \
"\x70\x73\x3a\x2f\x2f\x72\x61\x62\x62\x69\x74\x6d\x71\x2e\x63\x6f" \
"\x6d\x08\x70\x6c\x61\x74\x66\x6f\x72\x6d\x53\x00\x00\x00\x13\x45" \
"\x72\x6c\x61\x6e\x67\x2f\x4f\x54\x50\x20\x32\x35\x2e\x33\x2e\x32" \
"\x2e\x36\x07\x70\x72\x6f\x64\x75\x63\x74\x53\x00\x00\x00\x08\x52" \
"\x61\x62\x62\x69\x74\x4d\x51\x07\x76\x65\x72\x73\x69\x6f\x6e\x53" \
"\x00\x00\x00\x06\x33\x2e\x31\x32\x2e\x36\x00\x00\x00\x0e\x50\x4c" \
"\x41\x49\x4e\x20\x41\x4d\x51\x50\x4c\x41\x49\x4e\x00\x00\x00\x05" \
"\x65\x6e\x5f\x55\x53\xce";
	okwrite(connfd, CONNECTION_START, 516);

	// Ignore Connection.Start-Ok
	struct frame_t frame = read_frame(buffer, connfd);
	okread(connfd, buffer, frame.size + 1);

	static const char CONNECTION_TUNE[] = 
"\x01\x00\x00\x00\x00\x00\x0c\x00\x0a\x00\x1e\x07\xff\x00\x02\x00" \
"\x00\x00\x3c\xce";
	okwrite(connfd, CONNECTION_TUNE, 20);

	// Ignore Connection.Tune-Ok
	frame = read_frame(buffer, connfd);
	okread(connfd, buffer, frame.size + 1);

	// Ignore Connection.Open
	frame = read_frame(buffer, connfd);
	okread(connfd, buffer, frame.size + 1);

	static const char CONNECTION_OPEN_OK[] = 
"\x01\x00\x00\x00\x00\x00\x05\x00\x0a\x00\x29\x00\xce";
	okwrite(connfd, CONNECTION_OPEN_OK, 13);

	// Ignore Channel.OPEN
	frame = read_frame(buffer, connfd);
	okread(connfd, buffer, frame.size + 1);

	static const char CHANNEL_OPEN_OK[] = 
"\x01\x00\x01\x00\x00\x00\x08\x00\x14\x00\x0b\x00\x00\x00\x00\xce";
	okwrite(connfd, CHANNEL_OPEN_OK, 16); 
}

void connection_end_boilerplate(char* buffer, int connfd) {
	// Ignore Channel.Close
	struct frame_t frame = read_frame(buffer, connfd);
	okread(connfd, buffer, frame.size + 1);

	static const char CHANNEL_CLOSE_OK[] = 
"\x01\x00\x01\x00\x00\x00\x04\x00\x14\x00\x29\xce";
	okwrite(connfd, CHANNEL_CLOSE_OK, 12);

	// Ignore Connection.Close
	frame = read_frame(buffer, connfd);
	okread(connfd, buffer, frame.size + 1);

	static const char CONNECTION_CLOSE_OK[] =
"\x01\x00\x00\x00\x00\x00\x04\x00\x0a\x00\x33\xce";
	okwrite(connfd, CONNECTION_CLOSE_OK, 12);
}