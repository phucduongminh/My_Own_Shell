// INCLUDES
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
//#include <ncurses.h>
// for ifconfig
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
// for top
#include <ctype.h>
#include <dirent.h>
#include <time.h>
//for kill
#include <signal.h> 


#define shell_RL_BUFSIZE 1024

typedef struct counter {
  size_t dirs;
  size_t files;
} counter_t;

typedef struct entry {
  char *name;
  int is_dir;
  struct entry *next;
} entry_t;

int walk(const char *directory, const char *prefix, counter_t *counter) {
  entry_t *head = NULL, *current, *iter;
  size_t size = 0, index;

  struct dirent *file_dirent;
  DIR *dir_handle;

  char *full_path, *segment, *pointer, *next_prefix;

  dir_handle = opendir(directory);
  if (!dir_handle) {
    fprintf(stderr, "Cannot open directory \"%s\"\n", directory);
    return -1;
  }

  counter->dirs++;

  while ((file_dirent = readdir(dir_handle)) != NULL) {
    if (file_dirent->d_name[0] == '.') {
      continue;
    }

    current = malloc(sizeof(entry_t));
    current->name =
        strcpy(malloc(strlen(file_dirent->d_name) + 1), file_dirent->d_name);
    current->is_dir = file_dirent->d_type == DT_DIR;
    current->next = NULL;

    if (head == NULL) {
      head = current;
    } else if (strcmp(current->name, head->name) < 0) {
      current->next = head;
      head = current;
    } else {
      for (iter = head;
           iter->next && strcmp(current->name, iter->next->name) > 0;
           iter = iter->next)
        ;

      current->next = iter->next;
      iter->next = current;
    }

    size++;
  }

  closedir(dir_handle);
  if (!head) {
    return 0;
  }

  for (index = 0; index < size; index++) {
    if (index == size - 1) {
      pointer = "└── ";
      segment = "    ";
    } else {
      pointer = "├── ";
      segment = "│   ";
    }

    printf("%s%s%s\n", prefix, pointer, head->name);

    if (head->is_dir) {
      full_path = malloc(strlen(directory) + strlen(head->name) + 2);
      sprintf(full_path, "%s/%s", directory, head->name);

      next_prefix = malloc(strlen(prefix) + strlen(segment) + 1);
      sprintf(next_prefix, "%s%s", prefix, segment);

      walk(full_path, next_prefix, counter);
      free(full_path);
      free(next_prefix);
    } else {
      counter->files++;
    }

    current = head;
    head = head->next;

    free(current->name);
    free(current);
  }

  return 0;
}

// top
#define MAX_PROCESSES 1024
#define PROC_DIRECTORY "/proc/"

struct process_info {
  int pid;
  char command[256];
  char state;
  long unsigned int utime;
  long unsigned int stime;
  long unsigned int vsize;
};

void get_process_info(struct process_info *info, int pid) {
  char filename[256];
  snprintf(filename, sizeof(filename), PROC_DIRECTORY "%d/stat", pid);

  FILE *fp = fopen(filename, "r");
  if (!fp) {
    return;
  }

  int i = 0;
  char c;
  while ((c = fgetc(fp)) != EOF && i < 255) {
    if (c == '(' || i > 0) {
      info->command[i++] = c;
    }

    if (c == ')') {
      break;
    }
  }
  info->command[i - 1] = '\0';

  if (fscanf(fp, " %c", &info->state)) {
  };
  for (int i = 0; i < 13; i++) {
    if (fscanf(fp, " %*s")) {
    };
  }
  if (fscanf(fp, " %lu %lu %*s %*s %*s %*s %*s %lu", &info->utime, &info->stime,
             &info->vsize)) {
  };

  fclose(fp);
}