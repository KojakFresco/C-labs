#include "sort.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void insertion_sort(char keys[][50], char data[][50], int n) {
    for (int i = 1; i < n; i++) {
        char key[50]; strcpy(key, keys[i]);
        char d[50]; strcpy(d, data[i]);
        int j = i - 1;
        while (j >= 0 && strcmp(keys[j], key) > 0) {
            strcpy(keys[j+1], keys[j]);
            strcpy(data[j+1], data[j]);
            j--;
        }

        strcpy(keys[j+1], key);
        strcpy(data[j+1], d);
    }
}

int binary_search(char keys[][50], int n, const char* key) {
    int l = 0; int r = n - 1;
    while (r - l > 1) {
        int m = (l + r) / 2;
        if (strcmp(keys[m], key) == 0) {
            return m;
        }
        if (strcmp(keys[m], key) < 0) {
            l = m;
        } else {
            r = m;
        }
    }
    if (strcmp(keys[l], key) == 0) {
        return l;
    }
    if (strcmp(keys[r], key) == 0) {
        return r;
    }
    return -1;
}

void print_table(char keys[][50], char data[][50], int n) {
    for (int i = 0; i < n; i++) {
        printf("%-20s", keys[i]);
        printf("%-20s\n", data[i]);
    }
}

void fill_table(char keys[][50], char data[][50], int n, const char *type) {
    if (strcmp(type, "sorted") == 0) {
        for (int i = 0; i < n; i++) {
            sprintf(keys[i], "%d", i);
            sprintf(data[i], "data%d", i);
        }
    } else if (strcmp(type, "reversed") == 0) {
        for (int i = 0; i < n; i++) {
            sprintf(keys[i], "%d", n - 1 - i);
            sprintf(data[i], "data%d", n - 1 - i);
        }
    } else if (strcmp(type, "random") == 0) {
        for (int i = 0; i < n; i++) {
            int len = rand() % 10 + 1; // длина от 1 до 10
            for (int j = 0; j < len; j++) {
                keys[i][j] = 'a' + rand() % 26;
            }
            keys[i][len] = '\0';
            len = rand() % 10 + 1;
            for (int j = 0; j < len; j++) {
                data[i][j] = 'a' + rand() % 26;
            }
            data[i][len] = '\0';
        }
    }
}