#include "../linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node* new_ll() {
    Node* head = (Node*)malloc(sizeof(Node));
    head->value = NULL;
    head->next = NULL;
    head->prev = NULL;
    return head;
}

Node* insert_to_ll(Node* head, char* value, int index) {
    if (head->value == NULL) {
        if (index != 0) {
            fprintf(stderr, "Error: Wrong index for length 0!\n");
        }
        head->value = value;
        return head;
    }
    Node* cur = head;
    int counter = 0;
    while (counter < index && cur->next != head) {
        cur = cur->next;
        counter++;
    }
    if (counter + 1 == index) {
        cur = head;
    }
    else if (counter + 1 < index || index < 0) {
        fprintf(stderr, "Error: Wrong index for length %d!\n", counter + 1);
        exit(-1);
    }

    Node* new = (Node*)malloc(sizeof(Node));
    new->value = value;
    new->next = cur;
    if (cur->next == NULL) {
        cur->next = new;
        new->prev = cur;
    } else {
        new->prev = cur->prev;
        cur->prev->next = new;
    }
    cur->prev = new;
    return index > 0 ? head : new;
}

void print_elem(Node* head, int index) {
    if (head->value == NULL) {
        fprintf(stderr, "Error: The list is empty");
        exit(-1);
    }

    Node* cur = head;
    int counter = 0;
    while (counter < index && cur->next != head) {
        cur = cur->next;
        counter++;
    }
    if (counter < index || index < 0) {
        fprintf(stderr, "Error: Wrong index for length %d", counter + 1);
        exit(-1);
    }
    printf("%s\n", cur->value);
}

Node* delete_elem(Node* head, int index) {
    if (head->value == NULL) {
        printf("Error: The list is empty");
        return head;
    }

    Node* cur = head;
    int counter = 0;
    while (counter < index && cur->next != head) {
        cur = cur->next;
        counter++;
    }
    if (counter < index || index < 0) {
        fprintf(stderr, "Error: Wrong index for length %d", counter + 1);
        exit(-1);
    }

    if (cur->next == NULL) {
        cur->value = NULL;
    } else if (cur->next == cur->prev) {
        cur->prev->next = NULL;
        cur->next->prev = NULL;
    }  else {
        cur->prev->next = cur->next;
        cur->next->prev = cur->prev;
    }

    if (index == 0 && cur->next != NULL) {
        head = cur->next;
    }

    if (cur->value != NULL) {
        free(cur);
    }
    return head;
}

void print_ll(Node* head) {
    Node* cur = head;
    printf("[");
    if (cur->value != NULL) {
        do {
            printf("\"%s\" ", cur->value);
            cur = cur->next;
        } while (cur != head && cur != NULL);
    }
    printf("]\n");
}

// сортировка списка по убыванию значения элементов
Node* sort_ll(Node* head) {
    if (head->value == NULL) {
        printf("Error: The list is empty");
        return head;
    }
    Node* last = head->prev;
    while (last != head) {
        bool isSorted = true;
        Node* cur = head;
        while (cur != last) {
            if (strcmp(cur->value, cur->next->value) == -1) {
                isSorted = false;
                Node* first = cur->next;
                cur->prev->next = first;
                first->next->prev = cur;
                cur->next = first->next;
                first->prev = cur->prev;
                cur->prev = first;
                first->next = cur;
                if (first->next == head) head = first;
                if (first == last) last = cur;
            }
            else cur = cur->next;
        }
        if (isSorted) {
            break;
        }
        last = last->prev;
    }
    return head;
}