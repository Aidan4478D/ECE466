#ifndef _CODEGEN_H
#define _CODEGEN_H

#include "quads.h"
#include "ast.h"

void generate_asm(BASICBLOCK* bb);
void quad_to_asm(QUAD* quad);
char* get_memory_operand(QNODE *q);

#endif //_CODEGEN_H
