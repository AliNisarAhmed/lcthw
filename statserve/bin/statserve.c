#include "../src/statserve.h"
#include <liblcthw/dbg.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
  check(argc == 3, "USAGE: statserve port host");

  const char *host = argv[1];
  const char *port = argv[2];

  check(echo_server(host, port), "Failed to run echo server");

  exit(0);

error:
  exit(1);
}
