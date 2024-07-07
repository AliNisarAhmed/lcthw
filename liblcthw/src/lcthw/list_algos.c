#include "list_algos.h"

int List_bubble_sort(List *list, List_compare List_compare) {
  int swap;
  do {
    swap = 0;
    LIST_FOREACH(list, head, next, cur) {
      if (cur->next && List_compare(cur->value, cur->next->value) > 0) {
        // swap
        void *temp = cur->value;
        cur->value = cur->next->value;
        cur->next->value = temp;
        swap = 1;
      }
    }
  } while (swap == 1);

  return 0;
}

inline List *List_merge(List *left, List *right, List_compare comparer) {
  List *result = List_create();
  void *val = NULL;

  while (List_size(left) > 0 || List_size(right) > 0) {
    if (List_size(left) > 0 && List_size(right) > 0) {
      if (comparer(List_head(left), List_head(right)) <= 0) {
        val = List_popLeft(left);
      } else {
        val = List_popLeft(right);
      }
      List_pushRight(result, val);
    } else if (List_size(left) > 0) {
      val = List_popLeft(left);
      List_pushRight(result, val);
    } else {
      val = List_popLeft(right);
      List_pushRight(result, val);
    }
  }

  return result;
}

List *List_merge_sort(List *list, List_compare comparer) {
  List *result = NULL;

  if (List_size(list) <= 1) {
    return list;
  }

  List *left = List_create();
  List *right = List_create();
  int middle = List_size(list) / 2;

  LIST_FOREACH(list, head, next, cur) {
    if (middle > 0) {
      List_pushRight(left, cur->value);
    } else {
      List_pushRight(right, cur->value);
    }
    middle--;
  }

  List *sorted_left = List_merge_sort(left, comparer);
  List *sorted_right = List_merge_sort(right, comparer);

  if (sorted_left != left) {
    List_destroy(left);
  }
  if (sorted_right != right) {
    List_destroy(right);
  }

  result = List_merge(sorted_left, sorted_right, comparer);

  List_destroy(sorted_left);
  List_destroy(sorted_right);

  return result;
}

int List_insert_sorted(List *list, void *val, List_compare comparer) {
  ListNode *node = ListNode_create(list->head, NULL, val);
  list->head = node;
  list->size++;

  while (node->next && comparer(node->value, node->next->value) > 0) {
    void *temp = node->value;
    node->value = node->next->value;
    node->next->value = temp;
    node = node->next;
  }

  return 0;
}

List *List_split(List *list, int split_index) {
  if (!List_head(list) || split_index < 0 || split_index >= List_size(list)) {
    return NULL;
  }

  ListNode *cur = list->head;
  int count = 0;

  while (count != split_index) {
    count++;
    cur = cur->next;
  }

  List *result = List_create();
  result->head = cur->next;
  result->tail = list->tail;
  result->size = List_size(list) - split_index - 1;
  cur->next = NULL;
  list->tail = cur;
  list->size = split_index + 1;

  return result;
}
