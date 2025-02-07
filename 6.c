#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

int files_count = 0;

void search_files(const char *dirname, long min_size, long max_size, FILE *output) {
  DIR *dir = opendir(dirname);
  if (dir == NULL) {
    perror("Fail to open directory\n");
    exit(1);
  }
  
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }
      
      char path[1024];
      snprintf(path, sizeof(path), "%s/%s", dirname, entry->d_name);
      
      struct stat st;
      if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) {
          search_files(path, min_size, max_size, output);
        } else if (S_ISREG(st.st_mode)) {
          files_count++;
          if (st.st_size >= min_size && st.st_size <= max_size) {
            fprintf(output, "%s, %ld bytes\n", path, st.st_size);
          }
        }
      }
  }
  closedir(dir);
}

int main(int argc, char *argv[]) {
  if (argc != 5) {
    printf("Invalid args\n");
    exit(1);
  }
  
  long min_size = atol(argv[2]);
  long max_size = atol(argv[3]);
  
  FILE *output = fopen(argv[4], "w");
  if (output == NULL) {
    perror("Fail to open output file\n");
    exit(1);
  }
  
  search_files(argv[1], min_size, max_size, output);
  
  fclose(output);
  printf("Files scanned: %d\n", files_count);
  return 0;
}
