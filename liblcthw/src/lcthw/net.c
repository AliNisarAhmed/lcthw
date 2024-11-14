
#include <sys/socket.h>

#include "bstrlib.h"
#include "dbg.h"
#include "ringbuffer.h"
#include <fcntl.h>
#include <netdb.h>
#include <sys/select.h>
#include <unistd.h>

struct tagbstring NL = bsStatic("\n");
struct tagbstring CRLF = bsStatic("\r\n");


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

