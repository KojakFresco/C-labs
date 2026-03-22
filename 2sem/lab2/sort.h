#ifndef LABAC_SORT_H
#define LABAC_SORT_H

void insertion_sort(char keys[][50], char data[][50], int n);
int binary_search(char keys[][50], int n, const char* key);
void print_table(char keys[][50], char data[][50], int n);
void fill_table(char keys[][50], char data[][50], int n, const char *type);
#endif //LABAC_SORT_H