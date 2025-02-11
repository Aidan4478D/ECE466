#include "ast.h"
#include <stdlib.h>
#include <stdio.h>


ast_node_t* new_ident(char* str) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = IDENT_N;
    new_node->ident.name = str;

    /*fprintf(stderr, "ident is: %s\n", new_node->ident.name); */

    return new_node;
}


ast_node_t* new_string(STRTYPE str) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = STRING_N;
    if(str.type == STRING_T) new_node->string.string_literal = str.string_literal;
    if(str.type == CHAR_T) new_node->string.char_literal = str.char_literal;

    /*fprintf(stderr, "str lit is: %s\n", new_node->string.string_literal); */

    return new_node;
}


ast_node_t* new_number(NUMTYPE num) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = NUMBER_N;
    new_node->number.num_meta = num;

    /*fprintf(stderr, "detected a number, %d\n", new_node->number.num_meta.type); */

    return new_node;
}


ast_node_t* new_unop(int op, ast_node_t* node) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 
    
    new_node->type = UNOP_N;
    new_node->unop.node = node;
    new_node->unop.op = op;

    /*fprintf(stderr, "unop is: %d\n", op); */

    return new_node;
}


ast_node_t* new_binop(int op, ast_node_t* left, ast_node_t* right) {

    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = BINOP_N;
    new_node->binop.left = left;
    new_node->binop.right = right;
    new_node->binop.op = op;
    
    /*fprintf(stderr, "binop is: %d\n", op); */

    return new_node;
}


ast_node_t* new_ternop(ast_node_t* left, ast_node_t* center, ast_node_t* right) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 
    
    new_node->type = TERNOP_N;
    new_node->ternop.left = left;
    new_node->ternop.center = center;
    new_node->ternop.right = right;

    /*fprintf(stderr, "detected a ternary operator\n"); */

    return new_node;
}


ast_node_t* new_function(ast_node_t* left, ast_node_t* right) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = FUNCT_N;
    new_node->function.left = left;
    new_node->function.right = right;

    return new_node;
}

ast_node_t* new_assignop(int op, ast_node_t* left, ast_node_t* right) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = ASSIGNOP_N;
    new_node->assignop.left = left;
    new_node->assignop.right = right;
    new_node->assignop.op = op;
    
    /*fprintf(stderr, "detected an assignop\n"); */
    return new_node;
}
