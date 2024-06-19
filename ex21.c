#include "dbg.h"
#include <stdint.h>

int main(void) {
  debug("int_least_8_t: %d", INT_LEAST8_MIN);
  debug("INT_LEAST8_MAX: %d", INT_LEAST8_MAX);
  debug("Biggest number possible: %ul", INTMAX_MAX);
  debug("Smallest number possible: %ul", INTMAX_MIN);
  debug("Biggest unsiged number possible: %ul", UINTMAX_MAX);
}
