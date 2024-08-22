#include "../src/lcthw/bstrlib.h"
#include "minunit.h"

int testcmp(char **a, char **b) { return strcmp(*a, *b); }

char *test_bfromcstr() {

  bstring str = bfromcstr("Hello");
  mu_assert(str != NULL, "str is null?");
  mu_assert(str->slen == 5, "length should be 5");
  mu_assert(str->mlen >= 5, "mlen should atleast be 5");
  mu_assert(str->data[0] == 'H', "first char is H?");
  mu_assert(str->data[4] == 'o', "last char is o?");

  return NULL;
}

char *test_blk2bstr() {
  bstring str = blk2bstr("Hello", 5);
  mu_assert(str != NULL, "str is null?");
  mu_assert(str->slen == 5, "length should be 5");
  mu_assert(str->mlen >= 5, "mlen should atleast be 5");
  mu_assert(str->data[0] == 'H', "first char is H?");
  mu_assert(str->data[4] == 'o', "last char is o?");

  bstring str2 = bstrcpy(str);
  bdestroy(str);
  mu_assert(str2 != NULL, "str is null?");
  mu_assert(str2->slen == 5, "length should be 5");
  mu_assert(str2->mlen >= 5, "mlen should atleast be 5");
  mu_assert(str2->data[0] == 'H', "first char is H?");
  mu_assert(str2->data[4] == 'o', "last char is o?");

  return NULL;
}

char *test_bassign() {
  bstring s1 = bfromcstr("World");
  bstring s2 = bfromcstr("Hello");

  int res = bassign(s1, s2);

  mu_assert(res != BSTR_ERR, "error bassign");
  mu_assert(s2 != NULL, "str is null?");
  mu_assert(s2->slen == 5, "length should be 5");
  mu_assert(s2->mlen >= 5, "mlen should atleast be 5");
  mu_assert(s2->data[0] == 'H', "first char is H?");
  mu_assert(s2->data[4] == 'o', "last char is o?");

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_bfromcstr);
  mu_run_test(test_blk2bstr);
  mu_run_test(test_bassign);

  return NULL;
}

RUN_TESTS(all_tests);
