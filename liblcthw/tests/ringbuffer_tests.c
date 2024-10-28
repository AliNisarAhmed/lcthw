#include "../src/lcthw/ringbuffer.h"
#include "minunit.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "../src/lcthw/bstrlib.h"

static RingBuffer *b = NULL;

#define RINGBUFFER_SIZE 10

char *test_create() {
  b = RingBuffer_create(RINGBUFFER_SIZE);
  mu_assert(b != NULL, "RingBuffer is created");
  mu_assert(RingBuffer_empty(b) == 1, "new RingBuffer should be empty");
  mu_assert(RingBuffer_full(b) == 0, "new RingBuffer is not full");
  mu_assert(RingBuffer_available_data(b) == 0,
            "new RingBuffer has no data available");
  mu_assert(RingBuffer_available_space(b) == RINGBUFFER_SIZE,
            "new Ringbuffer should have full space");

  return NULL;
}

char *test_read_write() {
  int res = RingBuffer_write(b, "abcd", 4);
  mu_assert(res == 4, "Should write 4 bytes");
  mu_assert(RingBuffer_available_data(b) == 4, "available data is 4 bytes");
  mu_assert(RingBuffer_available_space(b) == 6, "available space is 6 bytes");
  mu_assert(RingBuffer_empty(b) == 0, "RingBuffer is no longer empty");
  mu_assert(RingBuffer_full(b) == 0, "RingBuffer is not full yet");

  char read_result[5] = {'\0'};
  res = RingBuffer_read(b, read_result, 4);
  mu_assert(res == 4, "should read 4 bytes");
  mu_assert(strncmp(read_result, "abcd", 4) == 0, "should read correct result");
  mu_assert(RingBuffer_empty(b) == 1,
            "RingBuffer is empty after all data is read");
  mu_assert(RingBuffer_full(b) == 0, "RingBuffer is not full");
  mu_assert(RingBuffer_available_data(b) == 0, "No data available");
  mu_assert(RingBuffer_available_space(b) == RINGBUFFER_SIZE,
            "Full space available");

  res = RingBuffer_write(b, "abcdefghij", 10);
  mu_assert(res == 10, "should write 10 bytes");
  mu_assert(RingBuffer_full(b) == 1, "RingBuffer is now full");
  mu_assert(RingBuffer_available_data(b) == 10, "full data available");
  mu_assert(RingBuffer_available_space(b) == 0, "no more space available");
  bstring bstr = RingBuffer_gets(b, 10);
  mu_assert(blength(bstr) == 10, "bstr length is 10");
  mu_assert(biseqcstr(bstr, "abcdefghij") == 1, "bstr is equal");
  mu_assert(RingBuffer_empty(b) == 1, "RingBuffer is empty");

  return NULL;
}

char *test_destroy() {
  RingBuffer_destroy(b);
  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_read_write);
  mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
