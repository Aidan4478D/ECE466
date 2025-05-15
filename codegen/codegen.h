#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "quads.h"
#include "ast.h"
#include "symtable.h"


extern list_t* string_literals;
extern FILE* out_file;
extern int str_label_cnt;
extern stack_t* scope_stack;
extern list_t* arg_list;

void generate_asm(BASICBLOCK* bb, char* fn_name);
void quad_to_asm(QUAD* quad);
char* get_memory_operand(QNODE *q);
void generate_global_vars(SYMTABLE* global);

#endif //_CODEGEN_H
