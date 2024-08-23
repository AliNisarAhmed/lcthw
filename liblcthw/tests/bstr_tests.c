#include "../src/lcthw/bstrlib.h"
#include "minunit.h"
#include <stdio.h>

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

char *test_bassigncstr() {
  bstring s1 = bfromcstr("World");
  int res = bassigncstr(s1, "Hello");

  mu_assert(res != BSTR_ERR, "error bassign");
  mu_assert(s1 != NULL, "str is null?");
  mu_assert(s1->slen == 5, "length should be 5");
  mu_assert(s1->mlen >= 5, "mlen should atleast be 5");
  mu_assert(s1->data[0] == 'H', "first char is H?");
  mu_assert(s1->data[4] == 'o', "last char is o?");

  s1 = bfromcstr("World");
  res = bassignblk(s1, "Hello", 5);

  mu_assert(res != BSTR_ERR, "error bassign");
  mu_assert(s1 != NULL, "str is null?");
  mu_assert(s1->slen == 5, "length should be 5");
  mu_assert(s1->mlen >= 5, "mlen should atleast be 5");
  mu_assert(s1->data[0] == 'H', "first char is H?");
  mu_assert(s1->data[4] == 'o', "last char is o?");

  res = bdestroy(s1);
  mu_assert(res != BSTR_ERR, "error bdestroy");

  return NULL;
}

char *test_bconcat() {
  bstring s1 = bfromcstr("Hello");
  bstring s2 = bfromcstr(" World");

  int res = bconcat(s1, s2);
  mu_assert(res != BSTR_ERR, "no error bconcat");
  mu_assert(res == BSTR_OK, "ok respnst bconcat");
  mu_assert(biseqcstr(s1, "Hello World"), "concat not successfull");
  mu_assert(blength(s1) == 11, "length is not correct");
  mu_assert(blength(s2) == 6, "s2 length is not correct");
  mu_assert(biseqcstr(s2, " World"), "s2 is not modified");

  return NULL;
}

char *test_bstricmp() {
  bstring s1 = bfromcstr("Hello");
  bstring s2 = bfromcstr("Hello");
  bstring s3 = bfromcstr("HellO");
  int res = bstrcmp(s1, s2);
  mu_assert(res == 0, "strings should be equal");
  res = bstrcmp(s1, s3);
  mu_assert(res != 0, "string should not be equal");
  s3 = bfromcstr("hello");
  res = bstricmp(s1, s3);
  mu_assert(res == 0, "strings should be equal case insensitive");

  return NULL;
}

char *test_binstr() {
  bstring s1 = bfromcstr("Hello World");
  bstring s2 = bfromcstr("llo W");
  int res = binstr(s1, 0, s2);
  mu_assert(res != BSTR_ERR, "no error when binstr");
  mu_assert(res == 2, "returns correct index");

  return NULL;
}

char *test_bformat() {
  bstring s1 = bformat("hello %i %s", 2, "world");
  mu_assert(biseqcstr(s1, "hello 2 world"), "bformat failed");

  return NULL;
}

char *test_bsplit() {
  struct bstrList *res = bsplit(bfromcstr("hello-2-world"), '-');
  mu_assert(res->qty == 3, "split in 3");
  mu_assert(biseq(res->entry[0], bfromcstr("")) == 0, "first empty");
  mu_assert(biseq(res->entry[1], bfromcstr("hello")) == 0, "hello");
  mu_assert(biseq(res->entry[2], bfromcstr("")) == 0, "second empty");
  /* mu_assert(biseq(res->entry[3], bfromcstr("2")) == 0, "2"); */
  /* mu_assert(biseq(res->entry[4], bfromcstr("")) == 0, "third empty"); */
  /* mu_assert(biseq(res->entry[5], bfromcstr("world")) == 0, "world"); */

  return NULL;
}

char *all_tests() {
  mu_suite_start();

  mu_run_test(test_bfromcstr);
  mu_run_test(test_blk2bstr);
  mu_run_test(test_bassign);
  mu_run_test(test_bassigncstr);
  mu_run_test(test_bconcat);
  mu_run_test(test_bstricmp);
  mu_run_test(test_binstr);
  mu_run_test(test_bformat);
  mu_run_test(test_bsplit);

  return NULL;
}

RUN_TESTS(all_tests);
