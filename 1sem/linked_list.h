#ifndef LABAC_LINKED_LIST_H
#define LABAC_LINKED_LIST_H

typedef struct Node {
    char* value;
    struct Node* prev;
    struct Node* next;
} Node;

Node* new_ll();
Node* insert_to_ll(Node* head, char* value, int index);
void print_elem(Node* head, int index);
Node* delete_elem(Node* head, int index);
void print_ll(Node* head);
Node* sort_ll(Node* head);
// сортировка списка по убыванию значения элементов

#endif