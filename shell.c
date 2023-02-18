#include "define.h"

// GLOBAL VARIABLES
char name[25];

// PROTOTYPES

int shell_cd(char **args);
int shell_help(char **args);
int shell_cls(char **args);
int shell_dog(char **args);
int shell_frem(char **args);
int shell_fmk(char **args);
int shell_copy(char **args);
int shell_hostnm(char **args);
int shell_path(char **args);
int shell_hd(char **args);
int shell_tl(char **args);
int shell_time(char **args);
int shell_history(char **args);
int shell_clearhis(char **args);
int shell_exit(char **args);
// int shell_color(char **args);
int shell_csvcv(char **args); // csv file convert
int shell_tree(char **args);
int shell_repeat(char **args);
int shell_where(char **args);

// system shell
int shell_execute(char **args);

// DATA

char *builtin_str[] = {
    "cd",       // change directory
    "help",     // help command
    "cls",      // clearscreen command
    "dog",      // cat command but with another name
    "frem",     // file remove
    "fmk",      // file make
    "copy",     // copy file command
    "hostnm",   // hostname(prints out the host name)
    "path",     // path (prints out the current directory)
    "hd",       // head(prints out the first line in a file)
    "tl",       // tail(prints out the last line in a file)
    "time",     // time(prints out the current time)
    "history",  // tells the history of your commands
    "clearhis", // clears the history of your commands
    "exit",     // exit(halts the program)
    //"color", //change color theme
    "csvcv",  // csv file convert
    "tree",   // allows users to view an easy-to-read list of files and folders
    "repeat", // repeat a command n times
    "where"   // search the directories

};

int (*builtin_func[])(char **) = {
    &shell_cd,    &shell_help, &shell_cls,     &shell_dog,      &shell_frem,
    &shell_fmk,   &shell_copy, &shell_hostnm,  &shell_path,     &shell_hd,
    &shell_tl,    &shell_time, &shell_history, &shell_clearhis, &shell_exit,
    &shell_csvcv, &shell_tree, &shell_repeat,  &shell_where

};

// FUNCTIONS
int shell_num_builtins() { return sizeof(builtin_str) / sizeof(char *); }

// change directory command
int shell_cd(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "shell: expected argument to \"cd\"\n");
    return 1;
  } else {
    if (chdir(args[1]) != 0) {
      perror("shell");
    }
  }
  return 1;
}

int shell_cls(char **args) {
  printf("\e[1;1H\e[2J");

  return 1;
}

int shell_dog(char **args) {

  if (args[1] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  char c;
  FILE *fptr = fopen(args[1], "r");

  if (!fptr) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  c = fgetc(fptr);
  while (c != EOF) {
    printf("%c", c);
    c = fgetc(fptr);
  }

  fclose(fptr);

  return 1;
}

int shell_frem(char **args) {

  if (args[1] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  if (remove(args[1]) == 0) {
    printf("\nDeleted %s succesfully\n\n", args[1]);
  }

  else {
    fprintf(stderr, "\n\nshell: Unable to delete %s\n\n", args[1]);
    return 1;
  }

  return 1;
}

int shell_fmk(char **args) {

  if (args[1] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a name for the file\n\n");
    return 1;
  }

  FILE *fptr;

  fptr = fopen(args[1], "a");

  if (!fptr) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  fclose(fptr);

  return 1;
}

int shell_copy(char **args) {

  // If there isn't a file specified it returns to the loop
  if (args[1] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  if (args[2] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  char ch;
  FILE *source, *target;

  source = fopen(args[1], "r");

  // if the file can't be opened
  if (!source) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  target = fopen(args[2], "a");

  // if the file can't be opened
  if (!target) {
    fclose(source);
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  // copies the first file into the second file
  while ((ch = fgetc(source)) != EOF) {
    fputc(ch, target);
  }

  // gives the user confirmation that the command worked
  printf("\nFile copied successfully.\n\n");

  // closes the files
  fclose(source);
  fclose(target);

  return 1;
}

int shell_hostnm(char **args) {

  printf("\n\n%s\n\n", name);

  return 1;
}

int shell_path(char **args) {

  char cwd[PATH_MAX];
  // gets the curent working directory
  if (getcwd(cwd, sizeof(cwd))) {
  };
  // and prints it
  printf("\n%s\n\n", cwd);

  return 1;
}

int shell_hd(char **args) {

  char *c = (char *)malloc(sizeof(char) * 100);

  // If there isn't a file specified it returns to the loop
  if (args[1] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a file\n\n");
    return 1;
  }

  FILE *fptr;

  fptr = fopen(args[1], "r");

  // If the file can't be opened
  if (!fptr) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  if (!c) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  if (fscanf(fptr, "%[^\n]", c)) {
  };
  printf("%s", c);

  // frees the 'c' variable and closes the file
  free(c);
  fclose(fptr);

  return 1;
}

int shell_tl(char **args) {
  char tmp[1024];

  FILE *fptr = fopen(args[1], "r");

  while (!feof(fptr)) {
    if (fgets(tmp, 1024, fptr)) {
    };
  }

  printf("\n%s\n", tmp);
}

int shell_time(char **args) {
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  printf("Current local date and time: %s", asctime(timeinfo));

  return 1;
}

int shell_history(char **args) {
  char c;
  FILE *fptr;
  fptr = fopen("./history.txt", "r");

  if (!fptr) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  printf("\nHistory of commands: \n");

  c = fgetc(fptr);
  while (c != EOF) {
    printf("%c", c);
    c = fgetc(fptr);
  }

  printf("\n");

  fclose(fptr);

  return 1;
}

int shell_clearhis(char **args) {
  FILE *fptr;

  fptr = fopen("./history.txt", "w");

  if (!fptr) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    return 1;
  }

  fclose(fptr);

  return 1;
}

// help command
int shell_help(char **args) {
  int i;
  printf("Shell\n");
  printf("The following are built in:\n");

  // loops through every command in the array
  for (i = 0; i < shell_num_builtins(); i++) {
    printf("  %s\n", builtin_str[i]);
  }

  return 1;
}

// exit command
int shell_exit(char **args) {
  // exits the whole program
  return 0;
}

// color
/*int shell_color(char **args){

  if(args[1] == NULL||args[2] == NULL){
    fprintf(stderr, "\n\nshell: please input text && background color\n\n");
    return 1;
  }

  char color1[20],color2[20];
  strcpy(color1,argv[1]);
  strcpy(color2,argv[2]);

  initscr();
  start_color();
  init_pair(1, color1, color2);//COLOR_RED, COLOR_BLACK
  bkgd(COLOR_PAIR(1));
  refresh();
  getch();
  endwin();

  fclose(fptr1);
  fclose(fptr2);

  return 1;
}*/

// convert csv file
int shell_csvcv(char **args) {
  if (args[1] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a input file\n\n");
    return 1;
  }

  if (args[2] == NULL) {
    fprintf(stderr, "\n\nshell: please provide a output file\n\n");
    return 1;
  }
  FILE *infile, *outfile;
  char line[100];
  char *fields[10];
  char field_data[10][10];
  int num_fields;
  int i, j;

  infile = fopen(args[1], "r");
  outfile = fopen(args[2], "w");

  while (fgets(line, 100, infile) != NULL) {
    num_fields = 0;
    fields[num_fields] = strtok(line, ",");

    while (fields[num_fields] != NULL) {
      num_fields++;
      fields[num_fields] = strtok(NULL, ",");
    }

    for (i = 0; i < num_fields; i++) {
      strcpy(field_data[i], fields[i]);
      for (j = strlen(field_data[i]); j < 10; j++) {
        field_data[i][j] = ' ';
      }
      field_data[i][10] = '\0';
      fputs(field_data[i], outfile);
    }

    fputc('\n', outfile);
  }

  fclose(infile);
  fclose(outfile);

  return 1;
}

// tree command
// ex. $tree Music
int shell_tree(char **args) {
  char *directory = args[1] != NULL ? args[1] : ".";
  fprintf(stderr, "%s\n", directory);
  counter_t counter = {0, 0};
  walk(directory, "", &counter);

  fprintf(stderr, "\n%zu directories, %zu files\n",
          counter.dirs ? counter.dirs - 1 : 0, counter.files);
  return 1;
}

// repeat command
int shell_repeat(char **args) {
  int i, num_repeats, bufsize = shell_RL_BUFSIZE;

  if (args[1] != NULL && args[2] != NULL) {
    fprintf(stderr, "Usage: repeat COMMAND\n");
  } else {
    fprintf(stderr, "\n\nshell: input repeat times and command\n\n");
    return 1;
  }

  num_repeats = atoi(args[1]);

  char **tokens = malloc(bufsize * sizeof(char *));
  int position = 0;
  i = 2;

  while (args[i] != NULL) {
    tokens[position] = args[i];
    position++;
    i++;
  }
  tokens[position] = NULL;

  for (i = 0; i < num_repeats; i++) {
    shell_execute(tokens);
    fprintf(stderr, "\n\n");
  }

  return 1;
}

// where command
int shell_where(char **args) {
  char *path, *token, *search_path;
  char path_separator[2] = ":";
  char filename[1024];

  if (args[1] == NULL) {
    fprintf(stderr, "\n\nshell: input folder/file name \n\n");
    return 1;
  }

  path = getenv("PATH");
  if (!path) {
    fprintf(stderr, "Error: PATH environment variable not set\n");
    return 1;
  }

  search_path = strdup(path);
  token = strtok(search_path, path_separator);
  while (token) {
    snprintf(filename, sizeof(filename), "%s/%s", token, args[1]);
    if (access(filename, X_OK) == 0) {
      printf("%s\n", filename);
      return 1;
    }
    token = strtok(NULL, path_separator);
  }

  fprintf(stderr, "Error: %s not found in PATH\n", args[1]);
  return 1;
}

// launches the commands using pid
int shell_launch(char **args) {
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("shell");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("shell");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

// executes the command
int shell_execute(char **args) {
  int i;

  if (args[0] == NULL) {
    // An empty command was entered.
    return 1;
  }

  // loops through every function in the array
  for (i = 0; i < shell_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }

  return shell_launch(args);
}

// takes in whatever the user inputed
char *shell_read_line(void) {
  int bufsize = shell_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  while (1) {

    c = getchar();

    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }

    position++;

    if (position >= bufsize) {
      bufsize += shell_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
        exit(EXIT_FAILURE);
      }
    }
  }
}

#define shell_TOK_BUFSIZE 64
#define shell_TOK_DELIM " \t\r\n\a"

// parses
char **shell_split_line(char *line) {
  int bufsize = shell_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char *));
  char *token;

  if (!tokens) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    exit(EXIT_FAILURE);
  }

  token = strtok(line, shell_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += shell_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char *));
      if (!tokens) {
        fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, shell_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

// main loop
void shell_loop(void) {
  char *line;
  char **args;
  int status;

  char cwd[PATH_MAX];

  do {
    if (getcwd(cwd, sizeof(cwd))) {
    };
    printf("\033[0;32mHome@%s:\033[0;34m%s/\033[0;32m$ ", name, cwd);
    printf("\033[0m");
    line = shell_read_line();

    FILE *fptr = fopen("./history.txt", "a");

    if (!fptr) {
      fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
      continue;
    }

    else {
      fprintf(fptr, "%s\n", line);
    }

    fclose(fptr);

    args = shell_split_line(line);
    status = shell_execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv) {

  // loads some files

  // GETS THE NAME OF THE USER TO BE USED IN THE MAIN LOOP
  FILE *fptr = fopen("name.txt", "r");

  // ERROR CHECKING
  if (!fptr) {
    fprintf(stderr, "\n\nshell: %s\n\n", strerror(errno));
    strcpy(name, "root");
  }

  // If the file can be opened it does this
  else {
    fseek(fptr, SEEK_END, 0);
    if (ftell(fptr)) {
    };
    rewind(fptr);
    // reads the name inside the file into a global variable
    if (fread(name, 25, 1, fptr)) {
    };
  }

  // Run command loop.
  shell_loop();

  return EXIT_SUCCESS;
}