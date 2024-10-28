#ifndef lcthw_Queue_h
#define lcthw_Queue_h

#include "list.h"

typedef List Queue;

#define Queue_create List_create
#define Queue_destroy List_destroy

#define Queue_send List_pushRight
#define Queue_recv List_popLeft
#define Queue_peek List_first

#define Queue_count List_size

#define QUEUE_FOREACH(S, V) LIST_FOREACH(S, head, next, V)

#endif
