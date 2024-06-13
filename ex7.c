#include <stdio.h>
#include <limits.h>

int main(int argc, char *argv[]) {
  int distance = 100;
  float power = 2.345f;
  double super_power = 56789.4532;
  char initial = 'A';
  char first_name[] = "Ali";
  char last_name[] = "Ahmed";

  first_name[2] = 97;
  first_name[3] = 'Z';

  printf("int: %d\n", distance);
  printf("float: %f\n", power);
  printf("double: %f\n", super_power);
  printf("char: %c\n", initial);
  printf("first name: %s\n", first_name);
  printf("last name: %s\n", last_name);


  int bugs = 100;
  double bug_rate = 1.2;

  printf("You have %d bugs at the imaginary rate of %f.\n", bugs, bug_rate);

  long universe_of_defects_1 = 1L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L;
  long long universe_of_defects_2 = 1L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L;
  long long universe_of_defects_3 = 1024LL;
  unsigned long long universe_of_defects_4 = 1024ULL;
  unsigned long long universe_of_defects = 1L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L * 1024L;
  printf("The entire universe has %llu bugs.\n", universe_of_defects);

  double expected_bugs = bugs * bug_rate;
  printf("You are expected to have %f bugs.\n", expected_bugs);

  double part_of_universe = expected_bugs / universe_of_defects;
  printf("The is only a %e portion of universe\n", part_of_universe);

  char nul_byte = 'z';
  int care_percentage = bugs * nul_byte;
  printf("Which means you should care %d%%.\n", care_percentage);
  printf("Value of 'nul_byte': %i\n", (int)nul_byte);

  

  return 0;
}
