
#include "dbg.h"
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

typedef struct DirInfo {
  char **files;
  int num_files;
} DirInfo;

DirInfo *list_files_in_dir(char *dir_path) {
  DirInfo *dir_info = malloc(sizeof(DirInfo));
  dir_info->num_files = 0;
  dir_info->files = malloc(1000);
  DIR *d;
  struct dirent *dir;
  d = opendir(dir_path);
  if (d) {
    while ((dir = readdir(d)) != NULL) {
      if (dir->d_type == DT_REG) {
        dir_info->files[dir_info->num_files++] = dir->d_name;
      }
    }
  }

  return dir_info;
}

int find_needle_in_file(char *needle, char *file_name) {
  char file_path[256];
  snprintf(file_path, sizeof(file_path), "./.logfind/%s", file_name);
  FILE *file = fopen(file_path, "r");
  check(file != NULL, "Error opening file");

  char *match;
  char buff[500];
  int flag = 0;
  int lines = 1;

  while (fgets(buff, sizeof(buff), file) != NULL) {
    size_t len = strlen(buff);
    if (len > 0 && buff[len - 1] == '\n') {
      buff[len - 1] = '\0';
    }
    match = strstr(buff, needle);

    if (match != NULL) {
      flag = 1;
      break;
    }
  }

  return flag;
error:
  return 0;
}

int check_or_flag(int argc, char *argv[]) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      return 1;
    }
  }

  return 0;
}

void match_with_and(int argc, char *argv[], DirInfo *dir_info) {
  char **needles = argv + 1;
  int match = 1;

  for (int i = 0; i < dir_info->num_files; i++) {
    for (int j = 0; j < argc - 1; j++) {
      char *needle = needles[j];
      if (strcmp(needle, "-o") == 0) {
        continue;
      }
      int result = find_needle_in_file(needle, dir_info->files[i]);
      if (result == 0) {
        match = 0;
        break;
      }
      if (!match) {
        break;
      }
    }
    if (match) {
      printf("./.logfind/%s has ALL of these strings\n", dir_info->files[i]);
    }
  }
}

void match_with_or(int argc, char *argv[], DirInfo *dir_info) {
  char **needles = argv + 1;
  int match = 0;

  for (int i = 0; i < dir_info->num_files; i++) {
    for (int j = 0; j < argc - 1; j++) {
      char *needle = needles[j];
      if (strcmp(needle, "-o") == 0) {
        continue;
      }
      int result = find_needle_in_file(needle, dir_info->files[i]);
      if (result) {
        match = 1;
        break;
      }
      if (match) {
        break;
      }
    }
    if (match) {
      printf("./.logfind/%s has one of these strings\n", dir_info->files[i]);
    }
  }
}

int main(int argc, char *argv[]) {
  int or_flag = check_or_flag(argc, argv);

  DirInfo *dir_info = list_files_in_dir("./.logfind");
  if (or_flag) {
    match_with_or(argc, argv, dir_info);
  } else {
    match_with_and(argc, argv, dir_info);
  }

  return 0;
error:
  return 1;
}
