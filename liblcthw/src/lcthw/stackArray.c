#include "stackArray.h"

StackArray *StackArray_create(size_t element_size, int initial_size) {
  return DArray_create(element_size, initial_size);
}
