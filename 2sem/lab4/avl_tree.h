// Реализовать следующие 4 операции:
// 1. Добавление узла к дереву по ключу (с балансировкой при
// необходимости).
// 2. Удаление узла из дерева по ключу (с балансировкой при
// необходимости)
// 3. Печать дерева
// 4. Поиск значения элемента по заданному ключу.
#ifndef LABAC_AVL_TREE_H
#define LABAC_AVL_TREE_H

#include <stdio.h>
#define KEY_SIZE 7

typedef struct avlNode {
    char key[KEY_SIZE];
    double data;
    int height;
    struct avlNode* parent;
    struct avlNode *left, *right;
} AVLNode;

typedef struct avlTree {
    AVLNode *root;
} AVLTree;

AVLTree* init_avl_tree();
void insert(AVLTree* tree, char key[KEY_SIZE], double data);
void delete_elem(AVLTree* tree, char key[KEY_SIZE]);
void print(AVLTree* tree, FILE* out);
void find(AVLTree* tree, char key[KEY_SIZE], FILE* out);
void destroy_avl_tree(AVLTree* tree);

#endif //LABAC_AVL_TREE_H
