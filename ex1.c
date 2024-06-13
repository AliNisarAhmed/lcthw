#include <stdio.h>

int main(int argc, char *argv[]) {
  int distance = 100;
  printf("You are %d miles away.\n", distance);
  printf("I am printing a string: %s\n", "Azlan Ali");
  printf("I am printing a float: %f\n", 0.24);
  printf("I am printing a short: %d\n", (short)24);
  printf("I am printing a long long: %llu\n", 24ll);
  printf("I am printing a size_t long long: %zu\n", sizeof(long long));
  printf("I am printing a size_t of int: %zu\n", sizeof(int));
  printf("I am printing a size_t of short: %zu\n", sizeof(short));

  return 0;
}
