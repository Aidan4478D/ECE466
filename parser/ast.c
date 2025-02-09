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

    fprintf(stderr, "got here!\n"); 

    new_node->type = IDENT_N;
    new_node->ident.name = str;

    fprintf(stderr, "created a new node with name %s\n", new_node->ident.name); 
    return new_node;
}


