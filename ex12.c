#include <stdio.h>
int main(void) {
  int areas[] = {10, 12, 13, 14, 20};
  char name[] = "Ali";
  char full_name[] = {'A', 'l', 'i', ' ', 'N', '.', ' ',
                      'A', 'h', 'm', 'e', 'd', '\0'};

  printf("The size of an int: %zu\n", sizeof(int));
  printf("The size of areas (int[]): %ld\n", sizeof(areas));
  printf("The number of ints in areas: %ld\n", sizeof(areas) / sizeof(int));
  printf("The first area is %d, the 2nd %d.\n", areas[0], areas[1]);

  printf("The size of char: %ld\n", sizeof(char));
  printf("The size of name (char[]): %ld\n", sizeof(name));
  printf("The number of chars in name: %ld\n", sizeof(name) / sizeof(char));

  printf("The size of full_name (char[]): %ld\n", sizeof(full_name));
  printf("The number of chars: %ld\n", sizeof(full_name) / sizeof(char));

  printf("name=\"%s\" and full_name=\"%s\"\n", name, full_name);

  name[1] = 'L';
  printf("name after change: %s\n", name);

  return 0;
}
