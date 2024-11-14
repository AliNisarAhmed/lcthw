#include <sys/socket.h>
#undef NDEBUG

#include "../src/lcthw/bstrlib.h"
#include "../src/lcthw/dbg.h"
#include "../src/lcthw/ringbuffer.h"
#include <fcntl.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>

struct tagbstring NL = bsStatic("\n");
struct tagbstring CRLF = bsStatic("\r\n");

int nonblock(int fd) {
  // F_GETFL returns the file access mode and the file status flags
  int flags = fcntl(fd, F_GETFL, 0);
  check(flags >= 0, "Invalid flags on nonblock");

  // F_SETFL sets the file status flags to the arg after it
  // so here we are setting fd status to nonbloc + whatever flags it already had
  int rc = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
  check(rc == 0, "Can't set nonblocking");

  return 0;

error:
  return -1;
}

int client_connect(char *host, char *port) {
  int rc = 0;
  struct addrinfo *addr = NULL;

  rc = getaddrinfo(host, port, NULL, &addr);
  check(rc == 0, "Failed to lookup %s:%s", host, port);

  // AF_INET is ipv4
  // SOCK_STREAD is TCP
  // 0 means choose default protocol
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  check(sock >= 0, "Cannot create a socket");

  debug("socket_addr: %s", addr->ai_addr->sa_data);

  rc = connect(sock, addr->ai_addr, addr->ai_addrlen);
  check(rc == 0, "Socket failed to connect");

  rc = nonblock(sock);
  check(rc == 0, "Can't set nonblocking");

  freeaddrinfo(addr);
  return sock;

error:
  freeaddrinfo(addr);
  return -1;
}

int read_some(RingBuffer *buffer, int fd, int is_socket) {
  int rc = 0;

  if (RingBuffer_available_data(buffer) == 0) {
    buffer->start = buffer->end = 0;
  }

  if (is_socket) {
    rc = recv(fd, RingBuffer_starts_at(buffer),
              RingBuffer_available_space(buffer), 0);
  } else {
    // read from unistd
    rc = read(fd, RingBuffer_starts_at(buffer),
              RingBuffer_available_space(buffer));
  }

  check(rc >= 0, "Failed to read from fd: %d", fd);

  RingBuffer_commit_write(buffer, rc);
  return rc;

error:
  return -1;
}

int write_some(RingBuffer *buffer, int fd, int is_socket) {
  int rc = 0;
  bstring data = RingBuffer_get_all(buffer);

  check(data != NULL, "Failed to get from the buffer");
  check(bfindreplace(data, &NL, &CRLF, 0) == BSTR_OK, "Failed to replace NL");

  if (is_socket) {
    rc = send(fd, bdata(data), blength(data), 0);
  } else {
    rc = write(fd, bdata(data), blength(data));
  }

  check(rc == blength(data), "Failed to write everything to fd: %d.", fd);
  bdestroy(data);

  return rc;

error:
  return -1;
}

int main(int argc, char *argv[]) {
  fd_set allreads;
  fd_set readmask;

  int socket = 0;
  int rc = 0;

  RingBuffer *in_rb = RingBuffer_create(1024 * 10); // 10kb
  RingBuffer *sock_rb = RingBuffer_create(1024 * 10);

  check(argc == 3, "USAGE: netclient host port");

  char *host = argv[1];
  char *port = argv[2];
  socket = client_connect(host, port);
  check(socket >= 0, "connect to %s:%s failed.", host, port);

  FD_ZERO(&allreads);
  FD_SET(socket, &allreads);
  FD_SET(0, &allreads);

  while (1) {
    readmask = allreads;
    rc = select(socket + 1, &readmask, NULL, NULL, NULL);
    check(rc >= 0, "Select failed");

    if (FD_ISSET(0, &readmask)) {
      rc = read_some(in_rb, 0, 0);
      check_debug(rc != -1, "Failed to read from stdin");
    }

    if (FD_ISSET(socket, &readmask)) {
      rc = read_some(sock_rb, socket, 0);
      check_debug(rc != -1, "Failed to read from socket");
    }

    while (!RingBuffer_empty(sock_rb)) {
      rc = write_some(sock_rb, 1, 0);
      check_debug(rc != -1, "Failed to write to stdout");
    }

    while (!RingBuffer_empty(in_rb)) {
      rc = write_some(in_rb, socket, 1);
      check_debug(rc != -1, "Failed to write to socket");
    }
  }

  return 0;

error:
  return -1;
}
