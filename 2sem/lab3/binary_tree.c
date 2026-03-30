#include "binary_tree.h"

#include <stdio.h>
#include <stdlib.h>

// static Node* add_node(Node* root, int data, Node* parent) {
//     if (root == NULL) {
//         Node* new = malloc(sizeof(Node));
//         if (new == NULL) {
//             printf("Memory allocation failed\n");
//             return NULL;
//         }
//         new->data = data;
//         new->left = NULL;
//         new->right = NULL;
//         new->parent = parent;
//         return new;
//     }
//     if (root->data > data) {
//         root->left = add_node(root->left, data, root);
//     } else if (root->data < data) {
//         root->right = add_node(root->right, data, root);
//     }
//     return root;
// }
//
static void print_node(Node* root, int depth) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf(" ");
    }
    switch (root->type) {
        case NUMBER:
            printf("%d\n", root->data.number);
            break;
        case UNARY:
        case OPERATION:
            printf("%c\n", root->data.operator);
            break;
        case VARIABLE:
            printf("%c\n", root->data.variable);
    }
    print_node(root->left, depth + 1);
    print_node(root->right, depth + 1);
}

Node* new_node(NodeType type, char* data, Node* parent, Node* left, Node* right) {
    Node* new = malloc(sizeof(Node));
    if (new == NULL) {
        printf("Memory allocation failed\n");
        return NULL;
    }
    new->type = type;
    switch (type) {
        case NUMBER:
            new->data.number = atoi(data);
            break;
        case UNARY:
        case OPERATION:
            new->data.operator = data[0];
            break;
        case VARIABLE:
            new->data.variable = data[0];
    }
    new->parent = parent;
    new->left = left;
    new->right = right;
    return new;
}

Tree* init_tree(Node* root) {
    Tree* tree = malloc(sizeof(Tree));
    if (tree == NULL) {
        printf("Memory allocation failed");
        return NULL;
    }
    tree->root = root;
    return tree;
}

// Tree* add(Tree* tree, NodeType type, int data) {
//     tree->root = add_node(tree->root, data, NULL);
//     return tree;
// }
//
void print(Tree* tree) {
    printf("left\nright\n");
    print_node(tree->root, 0);
}

// Tree* delete(Tree* tree, int data) {
//     tree->root = delete_node(tree->root, data);
//     return tree;
// }