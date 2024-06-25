#include "dbg.h"
#include <stdarg.h>
#include <stdlib.h>

#define MAX_DATA 100

int read_string(char **out_string, int max_buffer) {
  *out_string = calloc(1, max_buffer + 1);
  check_mem(*out_string);

  char *result = fgets(*out_string, max_buffer, stdin);
  check(result != NULL, "Input error.");

  return 0;
error:
  if (*out_string) {
    free(*out_string);
  }
  *out_string = NULL;
  return -1;
}

int read_int(long *out_int) {
  char *input = NULL;
  char *end = NULL;
  int rc = read_string(&input, MAX_DATA);
  check(rc == 0, "Failed to read number");

  *out_int = strtol(input, &end, 10);
  check((*end == '\0' || *end == '\n') && *input != '\0', "Invalid number: %s",
        input);

  free(input);
  return 0;

error:
  if (input) {
    free(input);
  }
  return -1;
}

int read_scan(const char *fmt, ...) {
  int i = 0;
  int rc = 0;
  long *out_int = NULL;
  char *out_char = NULL;
  char **out_string = NULL;
  int max_buffer = 0;

  va_list argp;
  va_start(argp, fmt);

  for (i = 0; fmt[i] != '\0'; i++) {
    if (fmt[i] == '%') {
      i++;
      switch (fmt[i]) {
      case '\0':
        sentinel("Invalid format, you ended the string with %%");
        break;
      case 'd':
        out_int = va_arg(argp, long *);
        rc = read_int(out_int);
        check(rc == 0, "Failed to read int.");
        break;
      case 'c':
        out_char = va_arg(argp, char *);
        *out_char = fgetc(stdin);
        break;
      case 's':
        max_buffer = va_arg(argp, int);
        out_string = va_arg(argp, char **);
        rc = read_string(out_string, max_buffer);
        check(rc == 0, "Failed to read string");
        break;
      default:
        sentinel("Invalid format");
      }
    } else {
      fgetc(stdin);
    }

    check(!feof(stdin) && !ferror(stdin), "Input error.");
  }

  va_end(argp);
  return 0;

error:
  return 1;
}

int println(char *str, ...) {
  va_list ptr;
  va_start(ptr, str);

  // char array to store token
  char token[100];

  // index of where to store the characters of str in token
  int k = 0;

  for (int i = 0; str[i] != '\0'; i++) {
    token[k++] = str[i];

    debug("token at start: %s", token);

    if (str[i + 1] == '%' || str[i + 1] == '\0') {
      token[k] = '\0';
      k = 0;

      if (token[0] != '%') {
        debug("token at 113: %s", token);
        fprintf(stdout, "%s", token);
      } else {
        int j = 1;
        char ch1 = token[j];

        // This loop required when format is like 0.2f, when ch1 = 'f' the loop
        // ends char ch1 = 0; while ((ch1 = token[j++]) < 58) {
        // }

        if (ch1 == 'd') {
          debug("token at printing d: %s", token);
          fprintf(stdout, token, va_arg(ptr, int));
        } else if (ch1 == 'c') {
          fprintf(stdout, token, va_arg(ptr, int));
        } else if (ch1 == 'f') {
          fprintf(stdout, token, va_arg(ptr, double));
        } else if (ch1 == 's') {
          debug("token at printing s: %s", token);
          fprintf(stdout, token, va_arg(ptr, char *));
        } else {
          fprintf(stdout, "%s", token);
        }
      }
    }
  }

  fprintf(stdout, "%s", "\n");

  va_end(ptr);
  return 0;

error:
  return 1;
}

int main(int argc, char *argv[]) {
  char *first_name = NULL;
  char initial = ' ';
  char *last_name = NULL;
  long age = 0;

  println("Hello, my name is %s and my age is %d, my sons name is %s", "Ali",
          37, "Azlan");

  printf("What's your first name? ");
  int rc = read_scan("%s", MAX_DATA, &first_name);
  check(rc == 0, "Failed to get first name");

  printf("What's your initial? ");
  rc = read_scan("%c\n", &initial);
  check(rc == 0, "Failed to get initial");

  printf("Whats your last name? ");
  rc = read_scan("%s", MAX_DATA, &last_name);
  check(rc == 0, "failed to get last name");

  printf("How old are you? ");
  rc = read_scan("%d", &age);
  check(rc == 0, "Failed to read age");

  printf("---- RESULTS ----\n");
  printf("First Name: %s", first_name);
  printf("Initial: '%c'\n", initial);
  printf("Last Name: %s", last_name);
  printf("Age: %ld\n", age);

  free(first_name);
  free(last_name);
  return 0;

error:
  return 0;
}
