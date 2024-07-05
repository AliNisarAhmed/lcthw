#include "list.h"
#include "dbg.h"

List *List_create() {
  List *list = calloc(1, sizeof(List));
  list->size = 0;
  list->head = NULL;
  list->tail = NULL;
  return list;
}

void List_destroy(List *list) {
  LIST_FOREACH(list, head, next, cur) {
    if (cur->prev) {
      free(cur->prev);
    }
  }

  free(list->tail);
  free(list);
}

void List_clear(List *list) {
  LIST_FOREACH(list, head, next, cur) { free(cur->value); }
}

void List_clear_destroy(List *list) {
  List_clear(list);
  List_destroy(list);
}

void List_pushRight(List *list, void *value) {
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->tail == NULL) {
    list->head = node;
    list->tail = node;
  } else {
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
  }

  list->size++;

error:
  return;
}

void *List_popRight(List *list) {
  ListNode *node = list->tail;
  return node != NULL ? List_remove(list, node) : NULL;
}

void List_pushLeft(List *list, void *value) {
  ListNode *node = calloc(1, sizeof(ListNode));
  check_mem(node);

  node->value = value;

  if (list->head == NULL) {
    list->head = node;
    list->tail = node;
  } else {
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
  }

  list->size++;

error:
  return;
}

void *List_popLeft(List *list) {
  ListNode *node = list->head;
  return node != NULL ? List_remove(list, node) : NULL;
}

void *List_remove(List *list, ListNode *node) {
  void *result = NULL;
  check(list->head && list->tail, "List is empty");
  check(node, "node can't be null");

  if (node == list->head && node == list->tail) {
    list->head = NULL;
    list->tail = NULL;
  } else if (node == list->head) {
    list->head = node->next;
    check(list->head != NULL, "Invalid list, somehow got a head that is NULL");
    list->head->prev = NULL;
  } else if (node == list->tail) {
    list->tail = node->prev;
    check(list->tail != NULL, "Invalid list, somehow got a next that is NULL");
    list->tail->next = NULL;
  } else {
    ListNode *before = node->prev;
    ListNode *after = node->next;
    after->prev = before;
    before->next = after;
  }

  list->size--;
  result = node->value;
  free(node);

  return result;

error:
  return result;
}

List *List_copy(List *list) {
  if (list->head == NULL) {
    return NULL;
  }

  List *new_list = calloc(1, sizeof(List));

  LIST_FOREACH(list, head, next, cur) { List_pushRight(new_list, cur->value); }

  return new_list;
}

void *List_replace(List *list, void *old_val, void *new_val) {
  LIST_FOREACH(list, head, next, cur) {
    if (cur->value == old_val) {
      void *result = cur->value;
      cur->value = new_val;
      return result;
    }
  }

  return NULL;
}
