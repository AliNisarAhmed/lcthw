#include <arpa/inet.h>
#include <asm-generic/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUF_SIZE 500
#define PORT "7899"

void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void) {
  char str[BUF_SIZE];

  struct addrinfo hints;
  struct addrinfo *result, *rp;

  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  char client_addr[INET6_ADDRSTRLEN];

  int yes = 1;

  memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  int status;
  int sfd; // socket file descriptor
  int comm_fd;

  if ((status = getaddrinfo(NULL, PORT, &hints, &result)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  for (rp = result; rp != NULL; rp = rp->ai_next) {
    sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (sfd == -1) {
      continue;
    }

    // prevent addr already in use errors
    if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      fprintf(stderr, "failed to setsockopt");
      exit(1);
    }

    if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0) {
      break;
    }
    close(sfd);
  }

  freeaddrinfo(result);

  if (rp == NULL) {
    fprintf(stderr, "could not bind\n");
    exit(1);
  }

  if (listen(sfd, 10) == -1) {
    fprintf(stderr, "Failed to listen");
    exit(1);
  }

  printf("Listening on port %s...\n", PORT);

  while (1) {
    addr_size = sizeof(their_addr);
    comm_fd = accept(sfd, (struct sockaddr *)&their_addr, &addr_size);

    if (comm_fd == -1) {
      fprintf(stderr, "Error accepting connections");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              client_addr, sizeof(client_addr));

    printf("server got connection from: %s\n", client_addr);

    int fork_res = fork();

    if (fork_res < 0) {
      fprintf(stderr, "Failed to fork");
      exit(1);
    } else if (fork_res > 0) {
      // main process
      printf("In the main process\n");
      /* close(comm_fd); */
    } else {
      // child process
      /* close(sfd); */
      while (1) {
        recv(comm_fd, str, BUF_SIZE, 0);
        printf("Echoing back - %s\n", str);
        send(comm_fd, str, strlen(str) + 1, 0);
      }
    }
  }

  return 0;
}
