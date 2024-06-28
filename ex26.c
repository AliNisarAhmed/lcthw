
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
  size_t needle_length = strlen(needle);
  char file_path[256];
  snprintf(file_path, sizeof(file_path), "./.logfind/%s", file_name);
  FILE *file = fopen(file_path, "r");
  check(file != NULL, "Error opening file");

  char file_char;
  int index = -1;

  while ((file_char = fgetc(file)) != EOF) {
    index++;
    if (file_char != needle[0]) {
      // if the first character doesn't match, don't bother with rest
      debug("No character match at index:%d, continue: %c", index, file_char);
      continue;
    }

    debug("found a character match at index: %d with char: %c", index,
          file_char);

    int matches = 1;

    char remaining[needle_length];
    remaining[needle_length - 1] = '\0';

    for (int i = 0; i < needle_length; i++) {
      file_char = fgetc(file);
      index++;
      remaining[i] = file_char;

      debug("rem: %s ;; index: %d ;; file_char: %c", remaining, index,
            file_char);

      if (file_char != needle[i + 1]) {
        // no match, now undo all fgetc to put the stream back to the next
        // character after mismatch
        debug("no match after initial, undoing ;; rem: %s ;; index: %d",
              remaining, index);
        for (int j = i; j >= 0; j--) {
          ungetc(remaining[j], file);
          index--;
        }
        debug("breaking out to search again: %d", index);
        break;
      }

      matches++;
      debug("increment matches: index: %d", index);
    }

    if (matches != needle_length) {
      debug("matches != needle_length, index: %d ;; matches: %d ;; "
            "needle_length: %zu",
            index, matches, needle_length);
      continue;
    }

    return index;
  }

  return -1;

error:
  return 0;
}

int main(int argc, char *argv[]) {
  char *needle = argv[1];
  DirInfo *dir_info = list_files_in_dir("./.logfind");
  for (int i = 0; i < dir_info->num_files; i++) {
    int result = find_needle_in_file(needle, dir_info->files[i]);
    if (result >= 0) {
      printf("File %s contains needle at index: %d\n", dir_info->files[i],
             result);
    }
  }

  return 0;
error:
  return 1;
}
