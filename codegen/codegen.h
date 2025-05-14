#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "quads.h"
#include "ast.h"

extern list_t* string_literals;
extern FILE* out_file;
extern int str_label_cnt;

void generate_asm(BASICBLOCK* bb, char* fn_name);
void quad_to_asm(QUAD* quad);
char* get_memory_operand(QNODE *q);

#endif //_CODEGEN_H
