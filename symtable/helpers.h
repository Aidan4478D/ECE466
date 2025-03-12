#ifndef _HELPERS_H
#define _HELPERS_H

#include "ast.h"
#include "symtable.h"

char* get_operator_string(int op); 
void print_ast_tree(ast_node_t *node, int indent); 
void print_sym_table(SYMTABLE *st);
char* get_symbol_type(int op);
char* get_storage_class(int op);

#endif //_HELPERS_H
