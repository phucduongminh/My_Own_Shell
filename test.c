#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dirent.h>



int main(int argc, char *argv[]) {
  char* directory = argc > 1 ? argv[1] : ".";
  printf("%s\n", directory);

  counter_t counter = {0, 0};
  walk(directory, "", &counter);

  printf("\n%zu directories, %zu files\n",
    counter.dirs ? counter.dirs - 1 : 0, counter.files);
  return 0;
}