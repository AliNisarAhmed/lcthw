#ifndef lcthw_List_Algos_h
#define lcthw_List_Algos_h

#include "list.h"
#include <stdlib.h>

typedef int (*List_compare)(void *s1, void *s2);

int List_bubble_sort(List *list, List_compare list_compare);

List *List_merge_sort(List *list, List_compare list_compare);

int List_insert_sorted(List *list, void* val, List_compare comparer);

List *List_split(List *list, int split_index);

#endif
