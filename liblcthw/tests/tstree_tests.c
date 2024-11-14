#include "../src/lcthw/bstrlib.h"
#include "../src/lcthw/tstree.h"
#include "minunit.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

TSTree *node = NULL;
char *value1 = "VALUE1";
char *value2 = "VALUE2";
char *value3 = "VALUE3";
char *value4 = "VALUE4";
char *value5 = "VALUE5";
char *reverse = "VALUE-R";
int traverse_count = 0;

struct tagbstring key1 = bsStatic("TEST");
struct tagbstring key2 = bsStatic("TEST2");
struct tagbstring key3 = bsStatic("TSET");
struct tagbstring key4 = bsStatic("T");
struct tagbstring key5 = bsStatic("ABC");

void print(void *value, void *data, char splitchar) {
  printf("PRINT: splitchar: %c\n", splitchar);
  printf("PRINT: value: %s\n", (char *)value);
  printf("PRINT: data: %s\n\n", (char *)data);
}

char *test_insert() {
  node = TSTree_insert(node, bdata(&key1), blength(&key1), value1);
  mu_assert(node != NULL, "Failed to insert into tst");
  printf("NODE_VALUE: %c\n", node->splitchar);

  node = TSTree_insert(node, bdata(&key2), blength(&key2), value2);
  mu_assert(node != NULL, "Failed to insert into tst with second name.");
  printf("NODE_VALUE: %c\n", node->splitchar);

  node = TSTree_insert(node, bdata(&key3), blength(&key3), reverse);
  mu_assert(node != NULL, "Failed to insert into tst with reverse name.");
  printf("NODE_VALUE: %c\n", node->splitchar);

  node = TSTree_insert(node, bdata(&key4), blength(&key4), value4);
  mu_assert(node != NULL, "Failed to insert into tst with second name.");
  printf("NODE_VALUE: %c\n", node->splitchar);

  node = TSTree_insert(node, bdata(&key5), blength(&key5), value5);
  mu_assert(node != NULL, "Failed to insert into tst with second name.");
  printf("NODE_VALUE: %c\n", node->splitchar);

  return NULL;
}

char *test_search_exact() {
  void *res = TSTree_search(node, bdata(&key5), blength(&key5));
  mu_assert(res == value5, "Got the wrong value back, should be value5");

  res = TSTree_search(node, "TABC", strlen("TABC"));
  mu_assert(res == NULL, "Should not find anything");

  return NULL;
}

char *test_search_prefix() {
  void *res = TSTree_search_prefix(node, bdata(&key5), blength(&key5));
  debug("result: %p, expected: %p", res, value5);
  mu_assert(res == value5, "should be value5 by prefix.");

  res = TSTree_search_prefix(node, bdata(&key5), 1);
  debug("result: %p, expected: %p", res, value5);
  mu_assert(res == value5, "should be value1");

  res = TSTree_search_prefix(node, "AE", strlen("AE"));
  debug("result: %p, expected: %p", res, NULL);
  mu_assert(res == NULL, "should be NULL");

  return NULL;
}

void TSTree_traverse_test_cb(void *value, void *data, char splitchar) {
  assert(value != NULL && "Should not get NULL value.");
  assert(data == value1 && "Expecting valueA as the data.");
  traverse_count++;
}

char *test_traverse() {
  traverse_count = 0;
  TSTree_traverse(node, TSTree_traverse_test_cb, value1);
  debug("traverse count is: %d", traverse_count);
  mu_assert(traverse_count == 5, "Didn't find 5 keys.");

  return NULL;
}

char *test_destroy() {
  TSTree_destroy(node);

  return NULL;
}
char *all_tests() {
  mu_suite_start();

  mu_run_test(test_insert);
  mu_run_test(test_search_exact);
  mu_run_test(test_search_prefix);
  /* mu_run_test(test_traverse); */
  /* mu_run_test(test_destroy); */

  return NULL;
}

RUN_TESTS(all_tests);
