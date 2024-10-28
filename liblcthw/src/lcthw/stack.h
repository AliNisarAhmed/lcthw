
#ifndef lcthw_Stack_h
#define lcthw_Stack_h

#include "list.h"

typedef List Stack;

#define Stack_create List_create
#define Stack_destroy List_destroy

#define Stack_push List_pushLeft
#define Stack_pop List_popLeft
#define Stack_peek List_first

#define Stack_count List_size

#define STACK_FOREACH(S, V) LIST_FOREACH(S, head, next, V)

#endif
