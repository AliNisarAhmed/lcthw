#include "../src/lcthw/list.h"
#include "../src/lcthw/list_algos.h"
#include "minunit.h"
#include <assert.h>
#include <string.h>

char *values[] = {"XXXX", "1234", "abcd", "xjvef", "NDSS"};

#define NUM_VALUES 5

List *create_words() {
  int i = 0;
  List *words = List_create();

  for (i = 0; i < NUM_VALUES; i++) {
    List_pushRight(words, values[i]);
  }

  return words;
}

List *create_integer_list() {
  List *ints = List_create();
  int i = 0;
  for (i = 0; i < NUM_VALUES; i++) {
    int *v = calloc(1, sizeof(int));
    *v = i + 3;
    List_pushRight(ints, v);
  }
  return ints;
}

int is_sorted(List *words) {
  LIST_FOREACH(words, head, next, cur) {
    if (cur->next && strcmp(cur->value, cur->next->value) > 0) {
      debug("%s %s", (char *)cur->value, (char *)cur->next->value);
      return 0;
    }
  }

  return 1;
}

char *test_bubble_sort() {
  List *words = create_words();

  int rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed");
  mu_assert(is_sorted(words), "Words are not sorted after bubble sort");

  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort of already sorted failed");
  mu_assert(is_sorted(words), "Words should be sort if already bubble sort");

  List_destroy(words);

  words = List_create(words);
  rc = List_bubble_sort(words, (List_compare)strcmp);
  mu_assert(rc == 0, "Bubble sort failed on empty list");
  mu_assert(is_sorted(words), "Words should be sorted if empty");

  List_destroy(words);

  return NULL;
}

char *test_merge_sort() {
  List *words = create_words();

  List *res = List_merge_sort(words, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Words are not sorted after merge sort");

  List *res2 = List_merge_sort(res, (List_compare)strcmp);
  mu_assert(is_sorted(res), "Should still be sorted after merge sort");
  List_destroy(res2);
  List_destroy(res);

  List_destroy(words);

  return NULL;
}

int integer_comparer(int *a, int *b) {
  int a_val = *a;
  int b_val = *b;
  if (a_val < b_val) {
    return -1;
  } else if (a_val == b_val) {
    return 0;
  } else {
    return 1;
  }
}

char *test_list_insert_after() {
  List *ints = create_integer_list();
  List_bubble_sort(ints, (List_compare)integer_comparer);
  int insert_val = 4;
  mu_assert(*(int *)(ints->head->value) == 3, "Head not changed");
  int result =
      List_insert_sorted(ints, &insert_val, (List_compare)integer_comparer);
  mu_assert(*(int *)(ints->head->value) == 3, "Head not changed");
  mu_assert(*(int *)(ints->head->next->value) == 4,
            "not inserted at correct position");
  mu_assert(List_size(ints) == 6, "Size not updated correctly");
  insert_val = 2;
  List_insert_sorted(ints, &insert_val, (List_compare)integer_comparer);
  mu_assert(*(int *)(ints->head->value) == 2, "Head should change");
  mu_assert(ints->size == 7, "Size not updated corrected, expected 6");
  return NULL;
}

char *test_list_split() {
  List *ints = create_integer_list();
  List *new_list = List_split(ints, 2);

  mu_assert(*(int *)List_head(ints) == 3, "Original list head changed");
  mu_assert(*(int *)List_head(new_list) == 6, "New list head expected 6");
  mu_assert(*(int *)List_tail(ints) == 5, "Original list tail not correct");
  mu_assert(*(int *)List_tail(new_list) == 7, "new list tail not correct");
  mu_assert(List_size(ints) == 3, "original list size incorrect");
  mu_assert(List_size(new_list) == 2, "new list size incorrect");

  return NULL;
}

char *all_tests() {

  mu_suite_start();

  mu_run_test(test_bubble_sort);
  mu_run_test(test_merge_sort);
  mu_run_test(test_list_insert_after);
  mu_run_test(test_list_split);

  return NULL;
}

RUN_TESTS(all_tests);
