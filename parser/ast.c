#include "ast.h"
#include <stdlib.h>
#include <stdio.h>

ast_node_t* new_binop(char op, ast_node_t* left, ast_node_t* right) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = BINOP_N;
    new_node->binop.left = left;
    new_node->binop.right = right;
    new_node->binop.op = op;

    return new_node;
}

ast_node_t* new_ident(char* str) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = IDENT_N;
    new_node->ident.name = str;

    fprintf(stderr, "ident is: %s\n", new_node->ident.name); 

    return new_node;
}

ast_node_t* new_string(STRTYPE str) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = STRING_N;
    if(str.type == STRING_T) new_node->string.string_literal = str.string_literal;
    if(str.type == CHAR_T) new_node->string.char_literal = str.char_literal;

    fprintf(stderr, "str lit is: %s\n", new_node->string.string_literal); 

    return new_node;
}

ast_node_t* new_number(NUMTYPE num) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = NUMBER_N;
    if(num.type == INT_T || num.type == LONG_T || num.type == LONGLONG_T) new_node->number._int = num._int;
    if(num.type == DOUBLE_T || num.type == LONGDOUBLE_T) new_node->number._double = num._double;
    if(num.type == FLOAT_T) new_node->number._float = num._float;
    
    fprintf(stderr, "detected a number\n"); 

    return new_node;
}


