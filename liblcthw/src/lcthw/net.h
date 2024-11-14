#ifndef net_h
#define net_h

#include "ringbuffer.h"

int read_some(RingBuffer *buffer, int fd, int is_socket);
int write_some(RingBuffer *buffer, int fd, int is_socket);

#endif // !net_h
