
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person {
  char *name;
  int age;
  int height;
  int weight;
};

struct Person *Person_create(char *name, int age, int height, int weight) {
  struct Person *who = malloc(sizeof(struct Person));
  assert(who != NULL);
  who->name = strdup(name);
  who->age = age;
  who->height = height;
  who->weight = weight;

  return who;
}

void increment_age(struct Person *who) { who->age += 10; }

void increment_age_no_pointer(struct Person who) { who.age += 10; }

void Person_destroy(struct Person *who) {
  assert(who != NULL);
  free(who->name);
  free(who);
}

void Person_print(struct Person *who) {
  printf("name: %s\n", who->name);
  printf("age: %d\n", who->age);
  printf("height: %d\n", who->height);
  printf("weight: %d\n", who->weight);
}

void Person_print_no_pointer(struct Person who) {
  printf("name: %s\n", who.name);
  printf("age: %d\n", who.age);
  printf("height: %d\n", who.height);
  printf("weight: %d\n", who.weight);
}

int main(int argc, char *argv[]) {
  struct Person *ali = Person_create("Ali Ahmed", 37, 187, 90);
  struct Person *azlan = Person_create("Azlan Ali", 2, 80, 15);
  struct Person samrah;
  samrah.name = "Samrah";
  samrah.age = 37;
  samrah.weight = 56;
  samrah.height = 156;

  printf("Ali is at memory location: %p\n", ali);
  Person_print(ali);

  printf("Azlan is at memory location: %p\n", azlan);
  Person_print(azlan);

  printf("Samrah is at memory location: %p\n", &samrah);
  Person_print_no_pointer(samrah);

  // Person_print(NULL);

  printf("---\n");

  ali->age += 20;
  ali->weight += 20;
  ali->height += 10;
  Person_print(ali);

  azlan->age += 30;
  azlan->weight += 30;
  Person_print(azlan);

  printf("age before: Ali: %d\n", ali->age);
  increment_age(ali);
  printf("age after: Ali: %d\n", ali->age);

  printf("age before: Samrah: %d\n", samrah.age);
  increment_age(ali);
  printf("age after: Samrah: %d\n", samrah.age);

  Person_destroy(ali);
  Person_destroy(azlan);

  return 0;
}
