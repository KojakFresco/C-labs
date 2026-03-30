#ifndef LABAC_BINARY_TREE_H
#define LABAC_BINARY_TREE_H

typedef enum {OPERATION, NUMBER, VARIABLE, UNARY} NodeType;

typedef struct node {
    NodeType type;
    union {
        int number;
        char operator;
        char variable;
    } data;
    struct node* left;
    struct node* right;
    struct node* parent;
} Node;

typedef struct {
    Node* root;
} Tree;

Node* new_node(NodeType type, char* data, Node* parent, Node* left, Node* right);

Tree* init_tree(Node* root);
// Tree* add(Tree* tree, NodeType type, int data);
void print(Tree* tree);
// Tree* delete(Tree* tree, int);
#endif //LABAC_BINARY_TREE_H