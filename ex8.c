#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("You have only 1 arg, exiting\n");
  } else if (argc > 1 && argc < 4) {
    printf("Here's your arguments: \n");
    for (int i = 0; i < argc; i++) {
      printf("%s ", argv[i]);
    }
    printf("\n");
  } else {
    printf("You have more than 4 args, exiting\n");
  }

  return 0;
}
