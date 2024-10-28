#ifndef lcthw_StackArray_h
#define lcthw_StackArray_h

#include "darray.h"

typedef DArray StackArray;

StackArray* StackArray_create(size_t element_size, int initial_size);
void *StackArray_peek(StackArray *s);

#define StackArray_destroy DArray_destroy

#define StackArray_push DArray_push
#define StackArray_pop DArray_pop
#define StackArray_peek DArray_last

#define StackArray_count DArray_count

#endif
