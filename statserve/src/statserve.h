#ifndef statserve_h
#define statserve_h

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <liblcthw/ringbuffer.h>
#include <liblcthw/bstrlib.h>

#define BACKLOG 10
#define BUF_SIZE 1024 * 10

int parse_line(bstring data, RingBuffer *send_rb);

int echo_server(const char *host, const char *port);

int server_listen(const char *host, const char *port);
int attempt_listen(struct addrinfo *addr);

#endif // !statserve_h
