#ifndef _HELPERS_H
#define _HELPERS_H

#include "ast.h"

char* get_operator_string(int op); 
void print_ast_tree(ast_node_t *node, int indent); 

#endif //_HELPERS_H
