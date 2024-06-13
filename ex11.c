#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, int len) {
  int i;
  for (i = 0; i < len; i++)
    printf(" %.2x", start[i]);
  printf("\n");
}

void show_float(float x) { show_bytes((byte_pointer)&x, sizeof(float)); }

void show_int(int x) { show_bytes((byte_pointer)&x, sizeof(int)); }

void show_pointer(void *x) { show_bytes((byte_pointer)&x, sizeof(void *)); }
int main(int argc, char *argv[]) {
  int numbers[4] = {0};
  char name[4] = {'a', 'a', 'a', 'a', 'a'};

  printf("numbers: %d %d %d %d \n", numbers[0], numbers[1], numbers[2],
         numbers[3]);
  printf("name chars: %c %c %c %c %c %c \n", name[0], name[1], name[2], name[3],
         name[4], name[5]);

  printf("name string: %s\n", name);

  numbers[0] = 1;
  numbers[1] = 2;
  numbers[2] = 3;
  numbers[3] = 4;

  printf("numbers initialized: %d %d %d %d \n", numbers[0], numbers[1],
         numbers[2], numbers[3]);

  name[0] = 'A';
  name[1] = 'l';
  name[2] = 'i';
  name[3] = 'Z';

  printf("name initialized: %c %c %c %c \n", name[0], name[1], name[2],
         name[3]);

  printf("name string: %s\n", name);

  char *another = "Ali";
  printf("another: %s\n", another);
  printf("another chars: %c %c %c %c\n", another[0], another[1], another[2],
         another[3]);

  printf("---- EXTRA ----\n");

  numbers[0] = 'A';
  numbers[1] = 'B';
  numbers[2] = 'C';
  numbers[3] = 'Z';

  printf("numbers with characters as chars: %c %c %c %c\n", numbers[0],
         numbers[1], numbers[2], numbers[3]);
  printf("numbers with characters as numbers: %d %d %d %d\n", numbers[0],
         numbers[1], numbers[2], numbers[3]);

  printf("name as numbers: %d %d %d %d\n", name[0], name[1], name[2], name[3]);

  int a_number;
  char *my_name = "abcd";
  memcpy(&a_number, my_name, 4);

  printf("a: %x\n", 97);
  printf("b: %x\n", 98);
  printf("c: %x\n", 99);
  printf("d: %x\n", 100);
  printf("a_number: after: %d\n", a_number);

  show_int(a_number);

  return 0;
}
