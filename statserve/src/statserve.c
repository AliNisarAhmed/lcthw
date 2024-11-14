// NOTE: to use <liblcthw/xyz> copy the xyz.o & xyz.h files to
// /usr/include/liblcthw (on pop-os, other OS may vary)

#include "statserve.h"
#include <liblcthw/dbg.h>
#include <liblcthw/net.h>
#include <liblcthw/ringbuffer.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

const int RB_SIZE = 1024 * 10;

void handle_sigchild(int input) {
  while (waitpid(-1, NULL, WNOHANG) > 0) {
  }
}

void client_handler(int client_fd) {
  int rc = 0;

  // store the input in a ring buffer
  /* RingBuffer *sock_rb = RingBuffer_create(RB_SIZE); */
  char str[BUF_SIZE] = {0};

  while (1) {
    recv(client_fd, &str, BUF_SIZE, 0);
    debug("received: %s", str);
    send(client_fd, str, strlen(str) + 1, 0);
  }

  // close the socket
  rc = close(client_fd);
  check(rc != -1, "Failed to close the socket");

error: // fallthrough
  exit(0);
}

int echo_server(const char *host, const char *port) {
  int rc = 0;
  struct sockaddr_in client_addr;
  socklen_t sin_size = sizeof(client_addr);
  int server_socket = 0;
  int client_fd = 0;

  struct sigaction sa = {.sa_handler = handle_sigchild,
                         .sa_flags = SA_RESTART | SA_NOCLDSTOP};

  check(host != NULL, "Invalid host");
  check(port != NULL, "Invalid port");

  // signal handling for child processes stops zombie processes from being
  // created and leaked
  sigemptyset(&sa.sa_mask);
  rc = sigaction(SIGCHLD, &sa, 0);

  server_socket = server_listen(host, port);
  check(server_socket >= 0, "bind to %s:%s failed", host, port);

  while (1) {
    client_fd =
        accept(server_socket, (struct sockaddr *)&client_addr, &sin_size);
    check(client_fd >= 0, "Failed to accept connection");

    debug("Client connected");

    rc = fork();
    if (rc == 0) {
      // child process
      close(server_socket); // don't need this in child process

      // handle the client
      client_handler(client_fd);
    } else {
      // server process
    }
  }

error: // fallthrough
  return -1;
}

int server_listen(const char *host, const char *port) {
  check(host != NULL, "Invalid host in server_listen");
  check(port != NULL, "Invalid port in server_listen");

  int rc = 0;
  int sockfd = -1; // default fail value

  struct addrinfo *info_result = NULL;
  struct addrinfo *next_p = NULL;

  // set up hints for getaddrinfo to use
  struct addrinfo hints;
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  // get the address info for host and port
  rc = getaddrinfo(NULL, port, &hints, &info_result);
  check(rc == 0, "Failed to get address info for connect");

  // cycle throught the available list to find one
  for (next_p = info_result; next_p != NULL; next_p = info_result->ai_next) {
    sockfd = attempt_listen(next_p);
    if (sockfd != -1) {
      break;
    }
  }

  // either we found one and were able to listen to, or nothing
  check(sockfd != -1, "All possible addresses failed.");

error: // fallthrough
  if (info_result) {
    freeaddrinfo(info_result);
  }

  // sockfd should point to valid socket at this point, or in case of error
  // should be -1
  return sockfd;
}

int attempt_listen(struct addrinfo *info) {
  check(info != NULL, "invalid addrinfo in attempt_listen");

  int rc = -1;
  int sockfd = -1; // default fail value
  int yes = 1;

  // create a socket with addrinfo
  sockfd = socket(info->ai_family, info->ai_socktype, info->ai_protocol);

  check(sockfd != -1, "failed to socket in attempt_listen");

  if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
    fprintf(stderr, "failed to setsockopt in attempt_listen");
    exit(1);
  }

  rc = bind(sockfd, info->ai_addr, info->ai_addrlen);

  check(rc == 0, "failed to bind in attempt_listen");

  rc = listen(sockfd, BACKLOG);
  check_debug(rc == 0, "Failed to listen to socket in attempt_listen");

error:
  return sockfd;
}
