#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "expressions.h"
#include "binary_tree.h"
#include "stack.h"

static int append_token_to_buffer(char** buf, size_t* capacity, size_t* length, const char* token) {
    size_t token_len = strlen(token);
    size_t required = *length + token_len + 2;
    if (required > *capacity) {
        size_t new_capacity = *capacity == 0 ? 64 : *capacity;
        while (new_capacity < required) new_capacity *= 2;
        char* resized = realloc(*buf, new_capacity);
        if (!resized) return 0;
        *buf = resized;
        *capacity = new_capacity;
    }
    memcpy(*buf + *length, token, token_len);
    *length += token_len;
    (*buf)[(*length)++] = ' ';
    (*buf)[*length] = '\0';
    return 1;
}

int is_integer(const char* str) {
    char* endptr;
    strtol(str, &endptr, 10);
    return *str != '\0' && *endptr == '\0';
}

int priority(char op) {
    switch (op) {
        case '+':
        case '-':
            return 2;
        case '*':
        case '/':
            return 3;
        case '~':
            return 4;
        case '(':
            return 0;
        case ')':
            return 1;
        default:
            return -1;
    }
}

int is_operand(char* str) {
    return priority(str[0]) == -1;
}

Node* simplify_node(Node* root) {
    if (root == NULL) return NULL;
    if (root->type == NUMBER || root->type == VARIABLE) {
        return root;
    }
    root->left = simplify_node(root->left);
    root->right = simplify_node(root->right);

    if (root->data.operator == '+') {
        if (root->left != NULL && root->left->type == NUMBER && root->left->data.number == 0) {
            if (root->right != NULL && root->right->type == NUMBER && root->right->data.number == 0) {
                free(root->right);
                free(root->left);
                free(root);
                return NULL;
            }
            Node* new_root = root->right;
            new_root->parent = root->parent;
            free(root->left);
            free(root);
            return new_root;
        }
        if (root->right != NULL && root->right->type == NUMBER && root->right->data.number == 0) {
            Node* new_root = root->left;
            new_root->parent = root->parent;
            free(root->right);
            free(root);
            return new_root;
        }
    }
    if (root->data.operator == '-') {
        if (root->left != NULL && root->left->type == NUMBER && root->left->data.number == 0) {
            if (root->right != NULL && root->right->type == NUMBER && root->right->data.number == 0) {
                free(root->right);
                free(root->left);
                free(root);
                return NULL;
            }
            Node* new_root = new_node(UNARY, "-", root->parent, NULL, root->right);
            free(root->left);
            free(root);
            return new_root;
        }
        if (root->right != NULL && root->right->type == NUMBER && root->right->data.number == 0) {
            Node* new_root = root->left;
            new_root->parent = root->parent;
            free(root->right);
            free(root);
            return new_root;
        }
    }
    return root;
}

int need_brackets(Node* child, Node* parent, int is_right) {
    if (!child || child->type != OPERATION) return 0;

    int p_child = priority(child->data.operator);
    int p_parent = priority(parent->data.operator);

    if (p_child < p_parent) return 1;

    if (is_right && p_child == p_parent) {
        if (parent->data.operator == '-' || parent->data.operator == '/')
            return 1;
    }

    return 0;
}

void print_node(Node* node) {
    if (node == NULL) return;
    switch (node->type) {
        case NUMBER:
            printf("%d", node->data.number);
            break;
        case VARIABLE:
            printf("%c", node->data.variable);
            break;
        case OPERATION:
            Node* left = node->left;
            if (need_brackets(left, node, 0)) printf("(");
            print_node(left);
            if (need_brackets(left, node, 0)) printf(")");

            printf("%c", node->data.operator);

            Node* right = node->right;
            if (need_brackets(right, node, 1)) printf("(");
            print_node(right);
            if (need_brackets(right, node, 1)) printf(")");
            break;
        case UNARY:
            printf("(-");
            print_node(node->right);
            printf(")");
    }
}

char** parse_expression(const char* exp) {
    char** tokens = NULL;
    int t_i = 0;

    for (int i = 0; exp[i] != '\0'; i++) {
        if (exp[i] == ' ') continue;

        char** new_tokens = realloc(tokens, (t_i + 1) * sizeof(char*));
        if (!new_tokens) {
            for (int j = 0; j < t_i; j++) free(tokens[j]);
            free(tokens);
            return NULL;
        }
        tokens = new_tokens;

        char* token = NULL;
        int d = 0;
        while (exp[i] != '\0' && isdigit(exp[i])) {
            char* new_token = realloc(token, d + 2);
            if (!new_token) {
                free(token);
                for (int j = 0; j < t_i; j++) free(tokens[j]);
                free(tokens);
                return NULL;
            }
            token = new_token;

            token[d] = exp[i];
            i++; d++;
        }
        if (!d) {
            token = malloc(2);
            if (!token) {
                for (int j = 0; j < t_i; j++) free(tokens[j]);
                free(tokens);
                return NULL;
            }

            token[0] = exp[i];
            token[1] = '\0';
        } else {
            token[d] = '\0';
            i--;
        }

        tokens[t_i] = malloc(strlen(token) + 1);
        if (!tokens[t_i]) {
            free(token);
            for (int j = 0; j < t_i; j++) free(tokens[j]);
            free(tokens);
            return NULL;
        }

        strcpy(tokens[t_i], token);
        free(token);
        t_i++;
    }

    char** new_tokens = realloc(tokens, (t_i + 1) * sizeof(char*));
    if (!new_tokens) {
        for (int j = 0; j < t_i; j++) free(tokens[j]);
        free(tokens);
        return NULL;
    }
    tokens = new_tokens;
    tokens[t_i] = NULL;

    return tokens;
}

char* postfix_notation(char** tokens) {
    if (!tokens) return NULL;

    Stack* stack = init_stack();
    if (!stack) return NULL;

    char* out = NULL;
    size_t out_capacity = 0;
    size_t out_length = 0;

    int i = 0;
    while (tokens[i] != NULL) {
        if (tokens[i][0] == '-' && (i == 0 || !is_operand(tokens[i-1]))) {
            push(stack, "~");
        } else if (is_operand(tokens[i])) {
            if (!append_token_to_buffer(&out, &out_capacity, &out_length, tokens[i])) {
                free(out);
                free_stack(stack);
                return NULL;
            }
        } else if (tokens[i][0] == '(') {
            push(stack, tokens[i]);
        } else if (tokens[i][0] == ')') {
            char* op = pop(stack);
            while (op != NULL && strcmp(op, "(") != 0) {
                if (!append_token_to_buffer(&out, &out_capacity, &out_length, op)) {
                    free(out);
                    free_stack(stack);
                    return NULL;
                }
                op = (char*)pop(stack);
            }
            if (!op) {
                printf("Error: wrong parenthesis");
                return NULL;
            }
        } else if (stack->size == 0 || priority(tokens[i][0]) > priority(*(char*)peek(stack))) {
            push(stack, tokens[i]);
        } else {
            char* op = peek(stack);
            while (op != NULL && priority(op[0]) >= priority(tokens[i][0])) {
                if (!append_token_to_buffer(&out, &out_capacity, &out_length, op)) {
                    free(out);
                    free_stack(stack);
                    return NULL;
                }
                pop(stack);
                op = peek(stack);
            }
            push(stack, tokens[i]);
        }
        i++;
    }
    while (stack->size != 0) {
        char* op = pop(stack);
        if (strcmp("(", op) == 0) {
            printf("Error: wrong parenthesis");
            free_stack(stack);
            return NULL;
        }
        if (!append_token_to_buffer(&out, &out_capacity, &out_length, op)) {
            free(out);
            free_stack(stack);
            return NULL;
        }
    }

    free_stack(stack);
    return out;
}

Tree* postfix_to_tree(char* exp) {
    Stack* stack = init_stack();
    if (!stack) return NULL;

    char* token = strtok(exp, " ");
    while (token != NULL) {
        if (is_operand(token)) {
            Node* node;
            if (is_integer(token)) {
                node = new_node(NUMBER, token, NULL, NULL, NULL);
            } else {
                node = new_node(VARIABLE, token, NULL, NULL, NULL);
            }
            if (!node) {
                free_stack(stack);
                return NULL;
            }
            push(stack, node);
        } else {
            Node* right = pop(stack);
            Node* node;
            if (token[0] != '~') {
                Node* left = pop(stack);
                node = new_node(OPERATION, token, NULL, left, right);
            } else {
                node = new_node(UNARY, "-", NULL, NULL, right);
            }
            if (!node) {
                free_stack(stack);
                return NULL;
            }
            push(stack, node);
        }
        token = strtok(NULL, " ");
    }
    Tree* tree = init_tree(pop(stack));
    if (stack->size != 0) {
        printf("ERROR: stack not empty after build\n");
    }
    free_stack(stack);

    return tree;
}

Tree* simplify(Tree* tree) {
    tree->root = simplify_node(tree->root);
    return tree;
}

void print_expression(Tree* tree) {
    print_node(tree->root);
}
