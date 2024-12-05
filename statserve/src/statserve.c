// NOTE: to use <liblcthw/xyz> copy the xyz.o & xyz.h files to
// /usr/include/liblcthw (on pop-os, other OS may vary)

#include "statserve.h"
#include <liblcthw/bstrlib.h>
#include <liblcthw/dbg.h>
#include <liblcthw/hashmap.h>
#include <liblcthw/net.h>
#include <liblcthw/ringbuffer.h>
#include <liblcthw/stats.h>
#include <netdb.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

struct tagbstring LINE_SPLIT = bsStatic(" ");
struct tagbstring CREATE = bsStatic("create");
struct tagbstring MEAN = bsStatic("mean");
struct tagbstring SAMPLE = bsStatic("sample");
struct tagbstring DUMP = bsStatic("dump");
struct tagbstring OK = bsStatic("OK\n");
struct tagbstring ERR = bsStatic("ERR\n");
struct tagbstring DNE = bsStatic("DNE\n"); // does not exist
struct tagbstring EXISTS = bsStatic("EXISTS");
const char LINE_ENDING = '\n';

const int RB_SIZE = 1024 * 10;

Hashmap *DATA = NULL;

// pre-declare Command so that it can be used in handler_cb
struct Command;
typedef int (*handler_cb)(struct Command *cmd, RingBuffer *send_rb);

typedef struct Command {
  bstring command;
  bstring name;
  bstring number;
  handler_cb handler;
} Command;

typedef struct Record {
  bstring name;
  Stats *stat;
} Record;

void handle_sigchild(int input) {
  while (waitpid(-1, NULL, WNOHANG) > 0) {
  }
}

void send_reply(RingBuffer *send_rb, bstring reply) {
  RingBuffer_puts(send_rb, reply);
}

int handle_create(Command *cmd, RingBuffer *send_rb) {
  int rc = 0;

  if (Hashmap_get(DATA, cmd->name)) {
    send_reply(send_rb, &EXISTS);
  } else {
    debug("create: %s %s", bdata(cmd->name), bdata(cmd->number));

    Record *info = calloc(sizeof(Record), 1);
    check_mem(info);

    info->name = bstrcpy(cmd->name);
    check_mem(info->name);

    Stats_sample(info->stat, atof(bdata(cmd->number)));

    rc = Hashmap_set(DATA, info->name, info);
    check(rc == 0, "Failed to add data to map");

    send_reply(send_rb, &OK);
  }

  return 0;

error:
  return -1;
}

int handle_sample(Command *cmd, RingBuffer *send_rb) {
  int rc = 0;
  Record *info = Hashmap_get(DATA, cmd->name);
  if (info == NULL) {
    send_reply(send_rb, &DNE);
  } else {
    Stats_sample(info->stat, atof(bdata(cmd->number)));
    bstring reply = bformat("%f\n", Stats_mean(info->stat));
    send_reply(send_rb, reply);
    bdestroy(reply);
  }
  return rc;
error:
  return -1;
}

int parse_command(struct bstrList *splits, Command *cmd) {
  cmd->command = splits->entry[0];
  if (biseq(cmd->command, &CREATE)) {
    cmd->name = splits->entry[1];
    cmd->number = splits->entry[2];
    cmd->handler = handle_create;
  } else if (biseq(cmd->command, &SAMPLE)) {
    cmd->name = splits->entry[1];
    cmd->number = splits->entry[2];
    cmd->handler = handle_sample;
  } else {
    sentinel("Failed to parse the command")
  }
  return 0;
error:
  return -1;
}

int parse_line(bstring data, RingBuffer *send_rb) {
  int rc = -1;
  Command cmd = {.command = NULL};

  // split data on line boundaries
  struct bstrList *splits = bsplits(data, &LINE_SPLIT);
  check(splits != NULL, "Bad data");

  // parse it into a command
  rc = parse_command(splits, &cmd);
  check(rc == 0, "Failed to parse command")

      // call the command handler for that command
      rc = cmd.handler(&cmd, send_rb);

error:
  if (splits) {
    bstrListDestroy(splits);
  }
  return rc;
}

// TODO:move to net.c in this project
bstring read_line(RingBuffer *input, const char line_ending) {
  int i = 0;
  bstring result = NULL;

  // read a character at a time from ring buffer
  for (i = 0; i < RingBuffer_available_data(input); i++) {
    if (input->buffer[i] == line_ending) {
      result = RingBuffer_gets(input, i);
      check(result, "Failed to get line from RingBuffer");
      check(RingBuffer_available_data(input) >= 1,
            "Not enough data in the RingBuffer after reading line");
      RingBuffer_commit_read(input, 1);
      break;
    }
  }
  return result;
error:
  return NULL;
}

void client_handler(int client_fd) {
  int rc = 0;

  RingBuffer *recv_rb = RingBuffer_create(RB_SIZE);
  RingBuffer *send_rb = RingBuffer_create(RB_SIZE);

  while (read_some(recv_rb, client_fd, 1) != -1) {
    bstring data = read_line(recv_rb, LINE_ENDING);
    check(data != NULL, "client closed");

    rc = parse_line(data, send_rb);
    check(rc == 0, "Failed to parse user input, closing...");

    bdestroy(data);

    if (RingBuffer_available_data(send_rb)) {
      write_some(send_rb, client_fd, 1);
    }
  }

  // close the socket
  rc = close(client_fd);
  check(rc != -1, "Failed to close the socket");
  exit(1);

error:
  if (recv_rb) {
    RingBuffer_destroy(recv_rb);
  }
  exit(0);
}

int setup_data_store() {
  DATA = Hashmap_create(NULL, NULL);
  check_mem(DATA);

  return 0;
error:
  return -1;
}

int echo_server(const char *host, const char *port) {
  int rc = 0;
  struct sockaddr_in client_addr;
  socklen_t sin_size = sizeof(client_addr);
  int server_socket = 0;
  int client_fd = 0;

  rc = setup_data_store();
  check(rc == 0, "Failed to set up the data store");

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

/* int main(void) { */
/*   debug("MAIN"); */
/*   Stats* s = Stats_create(); */
/* } */
