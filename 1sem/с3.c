#include <ctype.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

const int BUFFER_SIZE = 100;

int main() {
    FILE* input = fopen("input.txt", "r");
    if (input == NULL) {
        perror("open file");
        return 1;
    }

    char buffer[BUFFER_SIZE];
    int counter = 0;
    while(fgets(buffer, BUFFER_SIZE, input) != NULL) {
        int is_first = 1;
        char *first_int;
        char *last_int;

        for (char *t = strtok(buffer," \t\n"); t != NULL; t = strtok(nullptr, " \t\n")) {
            int f = 1;
            for (int i = 0; i < strlen(t); i++) {
                if (!isdigit(t[i])) {
                    f = 0;
                    break;
                }
            }
            if (f) {
                if (is_first) {
                    first_int = t;
                    is_first = 0;
                }
                last_int = t;
            }
        }
        printf("line %d: %s %s\n", ++counter, first_int, last_int);
    }

    fclose(input);

    return 1;
}