#include "../src/statserve.h"
#include "minunit.h"
#include <liblcthw/bstrlib.h>
#include <liblcthw/ringbuffer.h>

char *test_parse_line() {
  bstring data = bfromcstr("create stat1");
  RingBuffer *send_rb = RingBuffer_create(1024);
  int rc = parse_line(data, send_rb);

  mu_assert(rc == 0, "failed to parse line")

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_parse_line);

  return NULL;
}

RUN_TESTS(all_tests);
