#include <ctype.h>
#include <stdio.h>
#include <string.h>

void print_letters(char args[], int string_length);
void print_arguments(int argc, char *argv[]);

void print_arguments(int argc, char *argv[]) {
  int i = 0;
  for (i = 0; i < argc; i++) {
    int argument_length = strlen(argv[i]);
    print_letters(argv[i], argument_length);
  }
}

void print_letters(char args[], int string_length);

void print_letters(char arg[], int string_length) {
  int i = 0;

  printf("argument_length: %d \n", string_length);

  for (i = 0; i < string_length; i++) {
    char ch = arg[i];

    if (isalnum((int)ch) || isblank((int)ch)) {
      printf("'%c' == %d, ", ch, ch);
    }
  }

  printf("\n");
}

int main(int argc, char *argv[]) {
  print_arguments(argc, argv);
  return 0;
}
