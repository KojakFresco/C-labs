#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl_tree.h"

static AVLNode* _new_node(char key[KEY_SIZE], double data) {
    AVLNode* node = malloc(sizeof(AVLNode));
    if (!node) {
        fprintf(stderr, "Memory allocation failed");
        return NULL;
    }
    strncpy(node->key, key, 6);
    node->key[6] = '\0';
    node->data = data;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static inline int _height(const AVLNode* node) {
    return node == NULL ? 0 : node->height;
}

static inline int _max(const int a, const int b) {
    return a > b ? a : b;
}

AVLNode* _left_rotation(AVLNode* a) {
    AVLNode* b = a->right;
    a->right = b->left;
    b->left = a;

    a->height = 1 + _max(_height(a->left), _height(a->right));
    b->height = 1 + _max(_height(b->left), _height(b->right));
    return b;
}

AVLNode* _right_rotation(AVLNode* a) {
    AVLNode* b = a->left;
    a->left = b->right;
    b->right = a;

    a->height = 1 + _max(_height(a->left), _height(a->right));
    b->height = 1 + _max(_height(b->left), _height(b->right));
    return b;
}

AVLNode* _double_left_rotation(AVLNode* a) {
    AVLNode* b = a->right;
    AVLNode* c = b->left;
    b->left = c->right;
    c->right = b;
    a->right = c->left;
    c->left = a;

    a->height = 1 + _max(_height(a->left), _height(a->right));
    b->height = 1 + _max(_height(b->left), _height(b->right));
    c->height = 1 + _max(_height(c->left), _height(c->right));
    return c;
}

AVLNode* _double_right_rotation(AVLNode* a) {
    AVLNode* b = a->left;
    AVLNode* c = b->right;
    b->right = c->left;
    c->left = b;
    a->left = c->right;
    c->right = a;

    a->height = 1 + _max(_height(a->left), _height(a->right));
    b->height = 1 + _max(_height(b->left), _height(b->right));
    c->height = 1 + _max(_height(c->left), _height(c->right));
    return c;
}

AVLNode* _balance(AVLNode* root) {
    int balance = _height(root->right) - _height(root->left);
    if (-1 <= balance && balance <= 1) {
        return root;
    }
    if (balance > 1) {
        if (_height(root->right->left) <= _height(root->right->right)) {
            return _left_rotation(root);
        } else {
            return _double_left_rotation(root);
        }
    } else {
        if (_height(root->left->right) <= _height(root->left->left)) {
            return _right_rotation(root);
        } else {
            return _double_right_rotation(root);
        }
    }
}

static AVLNode* _insert_node(AVLNode* root, char key[KEY_SIZE], double data) {
    if (root == NULL) {
        return _new_node(key, data);
    }
    if (strcmp(key, root->key) < 0) {
        root->left = _insert_node(root->left, key, data);
    } else {
        root->right = _insert_node(root->right, key, data);
    }

    root->height = 1 + _max(_height(root->left), _height(root->right));

    return _balance(root);
}

static AVLNode* _delete_node(AVLNode* root, char key[KEY_SIZE]) {
    if (root == NULL) {
        fprintf(stderr, "AVLTree::delete_elem: no element with key '%s'\n", key);
        return NULL;
    }
    if (strcmp(key, root->key) < 0) {
        root->left = _delete_node(root->left, key);
    } else if (strcmp(key, root->key) > 0) {
        root->right = _delete_node(root->right, key);
    } else {
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }
        if (root->left == NULL) {
            AVLNode* new_root = root->right;
            free(root);
            return new_root;
        }
        if (root->right == NULL) {
            AVLNode* new_root = root->left;
            free(root);
            return new_root;
        }

        AVLNode* temp = root->right;
        while (temp->left != NULL) {
            temp = temp->left;
        }

        strcpy(root->key, temp->key);
        root->data = temp->data;
        root->right = _delete_node(root->right, temp->key);
    }

    root->height = 1 + _max(_height(root->left), _height(root->right));

    return _balance(root);
}

static void _print_node(AVLNode* node, const char* prefix, int is_left, FILE* file) {
    if (!node) return;

    if (node->right) {
        char new_prefix[256];
        snprintf(new_prefix, sizeof(new_prefix), "%s%s",
                 prefix, is_left ? "│   " : "    ");
        _print_node(node->right, new_prefix, 0, file);
    }

    fprintf(file, "%s", prefix);
    fprintf(file, "%s", is_left ? "└── " : "┌── ");
    fprintf(file, "%s (%.2f)\n", node->key, node->data);

    if (node->left) {
        char new_prefix[256];
        snprintf(new_prefix, sizeof(new_prefix), "%s%s",
                 prefix, is_left ? "    " : "│   ");
        _print_node(node->left, new_prefix, 1, file);
    }
}

static double _find_node(AVLNode* node, char key[KEY_SIZE], int* is_found) {
    if (node == NULL) {
        *is_found = -1;
        return 0;
    }
    if (strcmp(key, node->key) < 0) {
        return _find_node(node->left, key, is_found);
    }
    if (strcmp(key, node->key) > 0) {
        return _find_node(node->right, key, is_found);
    }
    if (strcmp(key, node->key) == 0) {
        *is_found = 1;
        return node->data;
    }

    return 0;
}

static void _destroy_nodes(AVLNode *node) {
    if (!node) {
        return;
    }

    _destroy_nodes(node->left);
    _destroy_nodes(node->right);
    free(node);
}

AVLTree* init_avl_tree() {
    AVLTree* tree = malloc(sizeof(AVLTree));
    tree->root = NULL;
    return tree;
}

void insert(AVLTree* tree, char key[KEY_SIZE], double data) {
    tree->root = _insert_node(tree->root, key, data);
}

void delete_elem(AVLTree* tree, char key[KEY_SIZE]) {
    tree->root = _delete_node(tree->root, key);
}

void print(AVLTree* tree, FILE* out) {
    _print_node(tree->root, "", 1, out);
}

void find(AVLTree* tree, char key[KEY_SIZE], FILE* out) {
    int is_found = 0;
    double res = _find_node(tree->root, key, &is_found);
    if (is_found == 1) {
        fprintf(out, "Element with key '%s' found: %lf\n", key, res);
    } else {
        fprintf(out, "Element with key '%s' not found\n", key);
    }
}

void destroy_avl_tree(AVLTree *tree) {
    if (!tree) {
        return;
    }

    _destroy_nodes(tree->root);
    free(tree);
}
