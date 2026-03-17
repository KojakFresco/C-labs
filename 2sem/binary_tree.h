#ifndef LABAC_BINARY_TREE_H
#define LABAC_BINARY_TREE_H

typedef struct node {
    struct node* left;
    struct node* right;
    struct node* parent;
    int data;
} Node;

typedef struct {
    Node* root;
} Tree;

Tree* init_tree();
Tree* add(Tree*, int);
void print(Tree*);
Tree* delete(Tree*, int);
int depth(Tree*);
#endif //LABAC_BINARY_TREE_H