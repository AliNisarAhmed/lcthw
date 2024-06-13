#include <stdio.h>
int main(int argc, char *argv[]) {
  int i = 0;


  char *states[] = {
    "California", "Alberta", "Texas", "Ontario", NULL
  };

  argv[1] = states[1];

  states[2] = argv[2];

  for (i = 1; i < argc; i++) {
    printf("arg %d: %s\n", i, argv[i]);
  }

  int num_states = 5;

  for (i = 0; i < num_states; i++) {
    printf("state %d: %s\n", i, states[i]);
  }

  return 0;
}
