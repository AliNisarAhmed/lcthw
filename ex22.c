#include "ex22.h"
#include "dbg.h"

int THE_SIZE = 1000;

// kinda inverse of extern: says that this variable is only used in 
// this .c file and should not be available to other parts of the program
static int THE_AGE = 37;

int get_age() { return THE_AGE; }

void set_age(int age) { THE_AGE = age; }

double update_ratio(double new_ratio) {
  // static in function means the variable acts as a static defined in the file
  // but it's only accessible from the function
  // it's like a "function scoped constant"
  // not used often
  static double ratio = 1.0;

  double old_ratio = ratio;
  ratio = new_ratio;

  return old_ratio;
}

void print_size() { log_info("I think size is: %d", THE_SIZE); }
