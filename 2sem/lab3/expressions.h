#ifndef LABAC_EXPRESSIONS_H
#define LABAC_EXPRESSIONS_H

#include "binary_tree.h"

char** parse_expression(const char* exp);
char* postfix_notation(char** tokens);
Tree* postfix_to_tree(char* exp);
Tree* simplify(Tree* tree);
void print_expression(Tree* tree);

#endif //LABAC_EXPRESSIONS_H