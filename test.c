#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LENGTH 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    char *filename = argv[1];
    char *path = getenv("PATH");
    char *dir = strtok(path, ":");
    while (dir != NULL) {
        char fullpath[1024];
        snprintf(fullpath, 1024, "%s/%s", dir, filename);
        if (access(fullpath, X_OK) == 0) {
            printf("%s\n", fullpath);
            return 0;
        }
        dir = strtok(NULL, ":");
    }
    printf("%s not found\n", filename);
    return 1;
}
