#ifndef _HELPERS_H
#define _HELPERS_H

#include "../ast.h"
#include "../symtable.h"

char* get_operator_string(int op); 
void print_ast_tree(ast_node_t *node, int indent); 
void print_sym_table(SYMTABLE *st);
void print_symbol(SYMTABLE *st, SYMBOL* sym);
char* get_symbol_type(int op);
char* get_storage_class(int op);
char* get_node_type(int op);
char* get_scope_name(int scope);
char* get_name_space(int op);
int get_opcode(ast_node_t* node);
void print_full_type(ast_node_t* node, int indent);
void print_type(ast_node_t *node);

#endif //_HELPERS_H
