#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int shell_csvcv(char **args); //csv file convert
int main(int argc, char *argv[]) {
    FILE *infile, *outfile;
    char line[100];
    char *fields[10];
    char field_data[10][10];
    int num_fields;
    int i, j;

    infile = fopen("input.csv", "r");
    outfile = fopen("output.fixed", "w");

    while (fgets(line, MAX_LINE_LENGTH, infile) != NULL) {
        num_fields = 0;
        fields[num_fields] = strtok(line, ",");

        while (fields[num_fields] != NULL) {
            num_fields++;
            fields[num_fields] = strtok(NULL, ",");
        }

        for (i = 0; i < num_fields; i++) {
            strcpy(field_data[i], fields[i]);
            for (j = strlen(field_data[i]); j < FIELD_WIDTH; j++) {
                field_data[i][j] = ' ';
            }
            field_data[i][FIELD_WIDTH] = '\0';
            fputs(field_data[i], outfile);
        }

        fputc('\n', outfile);
    }

    fclose(infile);
    fclose(outfile);

    return 0;
}