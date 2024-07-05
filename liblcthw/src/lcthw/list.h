#ifndef lcthw_List_h
#define lcthw_List_h

#include <stdlib.h>

struct ListNode;

typedef struct ListNode {
  struct ListNode *next;
  struct ListNode *prev;
  void *value;
} ListNode;

typedef struct List {
  int size;
  ListNode *head;
  ListNode *tail;
} List;


List *List_create();
void List_destroy(List *list);
void List_clear(List *list);
void List_clear_destroy(List *list);

#define List_size(A) ((A)->size)
#define List_head(A) ((A)->head != NULL ? (A)->head->value : NULL)
#define List_tail(A) ((A)->tail != NULL ? (A)->tail->value : NULL)

void List_pushRight(List *list, void *value);
void *List_popRight(List *list);

void List_pushLeft(List *list, void *value);
void *List_popLeft(List *list);

void *List_remove(List *list, ListNode *node);

List *List_copy(List *list);
void *List_replace(List *list, void* old_value, void *new_value);

#define LIST_FOREACH(L, S, M, V)                                               \
  ListNode *_node = NULL;                                                      \
  ListNode *V = NULL;                                                          \
  for (V = _node = L->S; _node != NULL; V = _node = _node->M)

#endif
