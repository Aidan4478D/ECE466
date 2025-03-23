#include "ast.h"
#include "helpers.h"

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
    new_node->string.str_meta = str;
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

ast_node_t* new_genop(NODETYPE type, int op, ast_node_t* left, ast_node_t* right) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = type;
    new_node->genop.left = left;
    new_node->genop.right = right;
    new_node->genop.op = op;
    
    /*fprintf(stderr, "detected an assignop\n"); */
    return new_node;
}

ast_node_t* new_element(ast_node_t* entry) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));

    node->type = ELEMENT_N;
    node->list.head = entry;
    node->list.next = NULL;
    return node;
}

ast_node_t* new_pointer(ast_node_t* next) {

    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));

    node->type = POINTER_N;
    node->pointer.next = next;
    
    return node;
}

ast_node_t* new_decl_spec(DECLTYPE decl_type, STGCLASS stg_class) {
    
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));

    node->type = DECLSPEC_N;
    if(decl_type) node->decl_spec.decl_type = decl_type;
    if(stg_class) node->decl_spec.stg_class = stg_class;

    return node; 
}

ast_node_t* new_list(ast_node_t* head) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));

    node->type = LIST_N;
    node->list.head = head;
    node->list.next = NULL;

    return node;
}

ast_node_t* append_item(ast_node_t* list, ast_node_t* entry) {
    if (!list) {
        return new_list(entry);
    }
    ast_node_t* current = list;

    while (current->list.next != NULL) {
        current = current->list.next;
    }
    current->list.next = new_element(entry);
    return list;
}


// array builds off of list
ast_node_t* new_array(ast_node_t* element_type, int size) {

    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = ARRAY_N;

    // implement array through list
    // need to have some sort of type indication
    node->array.element_type = element_type;
    node->array.size = size;  // if size is NULL, then it's an unsized array
    return node;
}

ast_node_t* combine_nodes(ast_node_t* base, ast_node_t* decl) {
    if (!decl)
        return base;
    if (!base)
        return decl;

    switch (decl->type) {
        case ARRAY_N:
            if (decl->array.element_type == NULL)
                decl->array.element_type = base;
            else
                decl->array.element_type = combine_nodes(base, decl->array.element_type);
            return decl;

        case POINTER_N: {
            ast_node_t* head = decl;
            ast_node_t* curr = decl;
            while (curr->pointer.next != NULL)
                curr = curr->pointer.next;
            curr->pointer.next = base;
            return head;
        }

        case FUNCT_N:
            if (decl->function.left == NULL)
                decl->function.left = base;
            else
                decl->function.left = combine_nodes(base, decl->function.left);
            return decl;

        case DECLSPEC_N:
            // declaration specifiers should be the base type
            // if base is another DECLSPEC_N or something else, we can combine them
            if (base->type == DECLSPEC_N) {
                // combine multiple declaration specifiers
                return append_item(base, decl);
            } else {
                // Attach the declspec as the base of the type
                return combine_nodes(decl, base);
            }

        default:
            return decl;
    }
}
