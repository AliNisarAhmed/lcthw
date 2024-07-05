#include "../src/lcthw/list.h"
#include "minunit.h"
#include <assert.h>

static List *list = NULL;
char *test1 = "test1 data";
char *test2 = "test2 data";
char *test3 = "test3 data";

char *test_create() {
  list = List_create();
  mu_assert(list != NULL, "Failed to create list");
  mu_assert(list->size == 0, "New list expected to be empty");
  mu_assert(list->head == NULL, "New list expected to have NULL head");
  mu_assert(list->tail == NULL, "New list expected to have NULL tail");

  return NULL;
}

char *test_pushRight_popRight() {
  List_pushRight(list, test1);
  mu_assert(List_tail(list) == test1, "Wrong tail value");
  mu_assert(List_size(list) == 1, "Wrong size, expected 1");

  List_pushRight(list, test2);
  mu_assert(List_tail(list) == test2, "Wrong tail value");
  mu_assert(List_size(list) == 2, "Wrong size, expected 2");

  List_pushRight(list, test3);
  mu_assert(List_tail(list) == test3, "Wrong last value");
  mu_assert(List_size(list) == 3, "Wrong size, expected 3");

  char *val = List_popRight(list);
  mu_assert(val == test3, "Wrong value on pop, expected test3");
  mu_assert(List_size(list) == 2, "Wrong size, expected 2");

  val = List_popRight(list);
  mu_assert(val == test2, "Wrong value on pop, expected test2");
  mu_assert(List_size(list) == 1, "Wrong size, expected 1");

  val = List_popRight(list);
  mu_assert(val == test1, "Wrong value on pop, expected test1");
  mu_assert(List_size(list) == 0, "Wrong size, expected 0");

  return NULL;
}

char *test_pushLeft() {
  List_pushLeft(list, test1);
  mu_assert(List_head(list) == test1, "expected test1");
  mu_assert(List_size(list) == 1, "Wrong size, expected 1");

  List_pushLeft(list, test2);
  mu_assert(List_head(list) == test2, "expected test2");
  mu_assert(List_size(list) == 2, "Wrong size, expected 2");

  List_pushLeft(list, test3);
  mu_assert(List_head(list) == test3, "expected test3");
  mu_assert(List_size(list) == 3, "Wrong size, expected 3");

  return NULL;
}

char *test_remove() {
  char *val = List_remove(list, list->head->next);
  mu_assert(val == test2, "Wrong element removed");
  mu_assert(List_size(list) == 2, "Wrong size, expected 2");
  mu_assert(List_head(list) == test3, "Wrong head after remove");
  mu_assert(List_tail(list) == test1, "Wrong tail after remove");

  return NULL;
}

char *test_popLeft() {
  mu_assert(List_size(list) != 0, "Wrong count before popLeft");
  char *val = List_popLeft(list);
  mu_assert(val == test3, "popLeft expected test3");
  mu_assert(List_size(list) == 1, "popLeft expected 1");
  val = List_popLeft(list);
  mu_assert(val == test1, "Wrong value on popLeft");
  mu_assert(List_size(list) == 0, "popLeft expected 0");
  mu_assert(List_head(list) == NULL, "list should be empty");
  mu_assert(List_tail(list) == NULL, "list should be empty");

  return NULL;
}

List *create_test_list() {
  List *new_list = calloc(1, sizeof(List));
  List_pushRight(new_list, test1);
  List_pushRight(new_list, test2);
  List_pushRight(new_list, test3);
  return new_list;
}

char *test_listCopy() {
  list = create_test_list();
  List *copy = List_copy(list);
  mu_assert(List_size(copy) == 3, "Size must be 3 after copy");
  mu_assert(List_size(list) == 3, "Size must be 3 after copy");
  mu_assert(List_head(list) == List_head(copy), "Head must be same after copy");
  mu_assert(List_tail(list) == List_tail(copy), "Tail must be same after copy");
  List_replace(list, test1, "new_head");
  mu_assert(List_head(copy) == test1, "copy is not mutated");
  List_pushRight(list, "test4");
  mu_assert(List_size(copy) == 3, "Size must remain 3 for copy");
  mu_assert(List_size(list) == 4, "Size must be 4 now");
  mu_assert(List_tail(list) != List_tail(copy),
            "Tail must be same after head remove");
  List_remove(list, list->head);
  mu_assert(List_size(copy) == 3, "Size must remain 3 for copy");
  mu_assert(List_size(list) == 3, "Size must be 3 now after head removed");
  mu_assert(List_head(list) != List_head(copy),
            "Head must be same after head remove");
  return NULL;
}

char *test_listReplace() {
  list = create_test_list();
  char *replaced_val = List_replace(list, test2, "new_val");
  mu_assert(strcmp(list->head->next->value, "new_val") == 0,
            "Value not replaced");
  mu_assert(List_size(list) == 3, "Size remains the same after replacement");
  mu_assert(List_replace(list, "non_existant", "not used") == NULL,
            "should not be able to replace not existing value");
  return NULL;
}

char *test_destroy() {
  List_clear_destroy(list);
  return NULL;
}

char *all_tests() {

  mu_suite_start();

  mu_run_test(test_create);
  mu_run_test(test_pushRight_popRight);
  mu_run_test(test_pushLeft);
  mu_run_test(test_remove);
  mu_run_test(test_popLeft);
  mu_run_test(test_listCopy);
  mu_run_test(test_listReplace);
  // mu_run_test(test_destroy);

  return NULL;
}

RUN_TESTS(all_tests);
