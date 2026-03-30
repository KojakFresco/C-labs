#ifndef LABAC_STACK_H
#define LABAC_STACK_H

typedef struct stack_node {
    struct stack_node *prev;
    void* data;
} StackNode;

typedef struct stack {
    StackNode *top;
    int size;
} Stack;

Stack* init_stack();
void push(Stack* stack, void* data);
void* pop(Stack* stack);
void* peek(Stack* stack);
void free_stack(Stack* stack);
#endif //LABAC_STACK_H