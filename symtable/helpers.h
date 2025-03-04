#ifndef _HELPERS_H
#define _HELPERS_H

#include "ast.h"
#include "symtable.h"

char* get_operator_string(int op); 
void print_ast_tree(ast_node_t *node, int indent); 
void print_sym_table(SYMTABLE *st);

#endif //_HELPERS_H
