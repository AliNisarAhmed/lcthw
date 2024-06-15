#include <stdio.h>

void print_names_and_ages_with_count(int *cur_age, char **cur_name, int count);
// notice difference in declaration vs definition (both ways possible)
void print_names_and_ages_with_count_2(int *ages, char **names, int count);

void print_names_and_ages_with_count(int *cur_age, char **cur_name, int count) {
  for (int i = 0; i < count; i++) {
    printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
  }
}

void print_names_and_ages_with_count_2(int ages[], char *names[], int count) {
  int *cur_age;
  char **cur_name;

  for (cur_age = ages, cur_name = names; (cur_age - ages) < count;
       cur_age++, cur_name++) {
    printf("%s is %d years old.\n", *cur_name, *cur_age);
  }
}

int main(int argc, char *argv[]) {
  int ages[] = {37, 37, 2, 32, 25};
  char *names[] = {"Ali", "Samrah", "Azlan", "Abbas", "Arshi"};

  int count = sizeof(ages) / sizeof(int);

  int i = 0;
  for (i = 0; i < count; i++) {
    printf("%s is %d years old.\n", names[i], ages[i]);
  }

  printf("---\n");

  // ----------------------------------------------------------------------------------
  int *cur_age = ages;
  char **cur_name = names;

  // deliberately pointing to wrong thing
  // int *cur_age = (int*)names;

  for (i = 0; i < count; i++) {
    printf("%s is %d years old.\n", *(cur_name + i), *(cur_age + i));
  }

  printf("---SAME as above with function---\n");

  print_names_and_ages_with_count(ages, names, count);

  // ----------------------------------------------------------------------------------

  printf("---\n");

  for (i = 0; i < count; i++) {
    printf("%s is %d years old.\n", cur_name[i], cur_age[i]);
  }

  printf("---\n");

  for (cur_name = names, cur_age = ages; (cur_age - ages) < count;
       cur_name++, cur_age++) {
    printf("%s is %d years old\n", *cur_name, *cur_age);
  }

  printf("---SAME as above with function---\n");

  print_names_and_ages_with_count_2(ages, names, count);

  printf("---\n");

  // ----------------------------------------------------------------------------------

  for (cur_name = &names[count - 1], cur_age = &ages[count - 1];
       (cur_age - ages) >= 0; cur_name--, cur_age--) {
    printf("%s is %d years old.\n", *cur_name, *cur_age);
  }

  printf("---PRINT ADDRESSES---\n");

  for (cur_name = names; (cur_name - names) < count; cur_name++) {
    printf("Pointer address: %p\n", cur_name);
    printf("Pointer value: %s\n", *cur_name);
  }

  printf("---PRINT ARGUMENTS---\n");

  char **arg = argv;

  for (i = 0; i < argc; i++) {
    printf("argument: %s\n", arg[i]);
  }

  printf("---PRINT ARGUMENTS 2---\n");

  for (arg = argv; (arg - argv) < argc; arg++) {
    printf("argument: %s\n", *arg);
  }
}
