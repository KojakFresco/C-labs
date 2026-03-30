#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

Stack* init_stack() {
    Stack* stack = (Stack*) malloc(sizeof(Stack));
    if (stack == NULL) {
        printf("Could not allocate stack");
        return NULL;
    }
    stack->top = NULL;
    stack->size = 0;
    return stack;
}

void push(Stack* stack, void* data) {
    if (stack == NULL) {
        printf("Stack is not initialized");
        return;
    }
    StackNode* node = (StackNode*) malloc(sizeof(StackNode));
    if (node == NULL) {
        printf("Could not allocate node");
        return;
    }
    node->data = data;
    node->prev = stack->top;
    stack->top = node;
    stack->size++;
}

void* pop(Stack* stack) {
    if (stack == NULL) {
        printf("Stack is not initialized");
        return NULL;
    }
    if (stack->top == NULL) {
        return NULL;
    }
    StackNode* node = stack->top;
    stack->top = node->prev;
    stack->size--;
    void* data = node->data;
    free(node);
    return data;
}

void* peek(Stack* stack) {
    if (stack == NULL) {
        printf("Stack is not initialized");
        return NULL;
    }
    if (stack->top == NULL) {
        return NULL;
    }
    return stack->top->data;
}

void free_stack(Stack* stack) {
    while (stack->size != 0) {
        void* data = pop(stack);
        free(data);
    }
    free(stack);
}
