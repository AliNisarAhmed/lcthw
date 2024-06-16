#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void die(const char *message) {
  if (errno) {
    perror(message);
  } else {
    printf("ERROR: %s\n", message);
  }

  exit(1);
}

typedef int (*compare_cb)(int a, int b);
typedef int *(*sorting_func)(int *numbers, int count, compare_cb cb);

int *bubble_sort(int *numbers, int count, compare_cb cmp) {
  int temp = 0;
  int i = 0;
  int j = 0;
  int *target = malloc(count * sizeof(int));

  if (target == NULL) {
    die("Memory error.");
  }

  memcpy(target, numbers, count * sizeof(int));

  for (i = 0; i < count; i++) {
    for (j = 0; j < count - 1; j++) {
      if (cmp(target[j], target[j + 1]) > 0) {
        temp = target[j + 1];
        target[j + 1] = target[j];
        target[j] = temp;
      }
    }
  }

  return target;
}

int *insertion_sort(int *numbers, int count, compare_cb cmp) {
  int temp = 0;
  int *target = malloc(count * sizeof(int));

  if (target == NULL) {
    die("Memory error");
  }

  memcpy(target, numbers, count * sizeof(int));

  for (int i = 1; i < count; i++) {
    for (int j = i; j > 0; j--) {
      if (cmp(target[j - 1], target[j]) > 0) {
        // swap
        temp = target[j];
        target[j] = target[j - 1];
        target[j - 1] = temp;
      } else {
        break;
      }
    }
  }

  return target;
}

int sorted_order(int a, int b) { return a - b; }

int reverse_order(int a, int b) { return b - a; }

int strange_order(int a, int b) {
  if (a == 0 || b == 0) {
    return 0;
  } else {
    return a % b;
  }
}

int wrong_func(char *a, int b) { return 1; }

void test_sorting(int *numbers, int count, sorting_func sorter,
                  compare_cb cmp) {
  int i = 0;
  int *sorted = sorter(numbers, count, cmp);

  if (!sorted) {
    die("Failed to sort as requested.");
  }

  for (i = 0; i < count; i++) {
    printf("%d", sorted[i]);
  }
  printf("\n");

  free(sorted);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    die("USAGE: ex18 4 3 1 2 5 6");
  }

  int count = argc - 1;
  int i = 0;
  char **inputs = argv + 1;

  int *numbers = malloc(count * sizeof(int));
  if (!numbers) {
    die("Memory error.");
  }

  for (i = 0; i < count; i++) {
    numbers[i] = atoi(inputs[i]);
  }

  test_sorting(numbers, count, bubble_sort, sorted_order);
  test_sorting(numbers, count, bubble_sort, reverse_order);
  test_sorting(numbers, count, bubble_sort, strange_order);

  printf("---\n");

  test_sorting(numbers, count, insertion_sort, sorted_order);
  test_sorting(numbers, count, insertion_sort, reverse_order);
  test_sorting(numbers, count, insertion_sort, strange_order);

  // test_sorting(numbers, count, wrong_func);
  // test_sorting(NULL, 2, NULL);

  free(numbers);

  return 0;
}
