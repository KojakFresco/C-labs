#include "binary_tree.h"
#include <stdio.h>
#include <stdlib.h>

static StackNode* add_node(StackNode* root, int data, StackNode* parent) {
    if (root == NULL) {
        StackNode* new = malloc(sizeof(StackNode));
        if (new == NULL) {
            printf("Memory allocation failed\n");
            return NULL;
        }
        new->data = data;
        new->left = NULL;
        new->right = NULL;
        new->parent = parent;
        return new;
    }
    if (root->data > data) {
        root->left = add_node(root->left, data, root);
    } else if (root->data < data) {
        root->right = add_node(root->right, data, root);
    }
    return root;
}

static void print_node(StackNode* root, int depth) {
    if (root == NULL) {
        return;
    }
    for (int i = 0; i < depth; i++) {
        printf(" ");
    }
    printf("%d\n", root->data);
    print_node(root->left, depth + 1);
    print_node(root->right, depth + 1);
}

static StackNode* delete_node(StackNode* root, int data) {
    if (root == NULL) {
        printf("No such element\n");
        return root;
    }
    if (root->data == data) {
        StackNode* new_root = root->right;
        if (new_root == NULL) {
            new_root = root->left;
            if (new_root == NULL) {
                return NULL;
            }
        } else {
            while (new_root->left != NULL) {
                new_root = new_root->left;
            }
            if (new_root != root->right) {
                new_root->parent->left = new_root->right;
                new_root->right = root->right;
            }
            new_root->left = root->left;
        }
        new_root->parent = root->parent;
        free(root);
        return new_root;
    } else if (data < root->data) {
        root->left = delete_node(root->left, data);
    } else {
        root->right = delete_node(root->right, data);
    }
    return root;
}

static int root_depth(StackNode* root, int cur_depth) {
    if (root == NULL) {
        return cur_depth;
    }
    int l_depth = root_depth(root->left, cur_depth + 1);
    int r_depth = root_depth(root->right, cur_depth + 1);
    return l_depth > r_depth ? l_depth : r_depth;
}

Tree* init_tree() {
    Tree* tree = malloc(sizeof(Tree));
    if (tree == NULL) {
        printf("Memory allocation failed");
        return NULL;
    }
    tree->root = NULL;
    return tree;
}

Tree* add(Tree* tree, int data) {
    tree->root = add_node(tree->root, data, NULL);
    return tree;
}

void print(Tree* tree) {
    print_node(tree->root, 0);
}

Tree* delete(Tree* tree, int data) {
    tree->root = delete_node(tree->root, data);
    return tree;
}

int depth(Tree* tree) {
    int depth = root_depth(tree->root, -1);
    if (depth == -1) {
        printf("The tree is empty\n");
        return 0;
    }
    return depth;
}