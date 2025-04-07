#include "ast.h"
#include "y.tab.h"
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
    node->decl_spec.decl_type = decl_type;
    node->decl_spec.stg_class = stg_class;

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
    if (!list) return new_list(entry);

    ast_node_t* current = list;

    while (current->list.next) {
        current = current->list.next;
    }

    current->list.next = new_element(entry);
    return list;
}


ast_node_t* new_array(ast_node_t* element_type, int size) {

    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = ARRAY_N;

    node->array.element_type = element_type;
    node->array.size = size;
    return node;
}


ast_node_t* new_param(ast_node_t* type, ast_node_t* ident) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = PARAM_N;

    node->parameter.type = type;    // not node->type, this is an ast_node that references a decl spec type (ex. INT)
    node->parameter.ident = ident;  // if size is NULL, then it's an unsized array

    return node;
}

ast_node_t* new_struct_union(int token, SYMBOL* sym) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = (token == STRUCT ? STRUCT_N : UNION_N);

    node->struct_union.sym = sym; //struct union just gonna be tracked through symbol

    return node;
}

// ensures that the identifier is attached to the AST node, whether it’s NULL (simple declarator) 
// or a chain like POINTER_N
ast_node_t* attach_ident(ast_node_t* node, char* ident) {
    if (node == NULL) return new_ident(ident);
    ast_node_t* current = node;

    while (1) {
        switch (current->type) {
            case POINTER_N:
                if (current->pointer.next == NULL) {
                    current->pointer.next = new_ident(ident);
                    return node;
                }
                current = current->pointer.next;
                break;
            case ARRAY_N:
                if (current->array.element_type == NULL) {
                    current->array.element_type = new_ident(ident);
                    return node;
                }
                current = current->array.element_type;
                break;
            default:
                // if we reach a leaf, assume its already set or not applicable
                return node;
        }
    }
}

// use mainly for printing but kept in here because it returns an ast node lol
ast_node_t* extract_ident(ast_node_t* node) {
    if (!node) return NULL;
    switch (node->type) {
        case IDENT_N: return node;
        case POINTER_N: return extract_ident(node->pointer.next);
        case ARRAY_N: return extract_ident(node->array.element_type);
        default: return NULL;
    }
}



ast_node_t* new_if(ast_node_t* exp, ast_node_t* statement) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = IF_N;

    node->if_node.expression = exp;
    node->if_node.statement = statement;

    return node;
}

ast_node_t* new_for(ast_node_t* init, ast_node_t* condition, ast_node_t* update, ast_node_t* statement) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = FOR_N;

    node->for_node.init = init;
    node->for_node.condition = condition;
    node->for_node.update = update;
    node->for_node.statement = statement;

    return node;
}

ast_node_t* new_while(ast_node_t* exp, ast_node_t* statement) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = WHILE_N;

    node->while_node.expression = exp;
    node->while_node.statement = statement;

    return node;
}

ast_node_t* new_switch(ast_node_t* exp, ast_node_t* statement) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = SWITCH_N;

    node->switch_node.expression = exp;
    node->switch_node.statement = statement;

    return node;
}

ast_node_t* new_label(SYMBOL* sym, ast_node_t* statement) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = LABEL_N;

    node->label_node.sym = sym;
    node->label_node.statement = statement;

    return node;
}

ast_node_t* new_goto(SYMBOL* sym) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = GOTO_N;

    node->goto_node.sym = sym;

    return node;
}

ast_node_t* new_return(ast_node_t* exp) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = RETURN_N;

    node->return_node.expression = exp;

    return node;
}


ast_node_t* new_switch_label(NODETYPE type, ast_node_t* name, ast_node_t* statement) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = type;

    node->switch_label.name = name;
    node->switch_label.statement = statement;

    return node;
}


ast_node_t* new_continue_break(NODETYPE type) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = type;

    return node;
}



ast_node_t* combine_nodes(ast_node_t* base, ast_node_t* decl) {
    if (!decl) return base;
    if (!base) return decl;

    switch (decl->type) {
        case ARRAY_N:
            if (!decl->array.element_type) decl->array.element_type = base;
            else decl->array.element_type = combine_nodes(base, decl->array.element_type);

            return decl;

        case POINTER_N: {
            if (!decl->pointer.next) decl->pointer.next = base;
            else decl->pointer.next = combine_nodes(base, decl->pointer.next);
        
            return decl;
        }

        case FUNCT_N:
            // If base is a pointer, it applies to the entire function
            if (base->type == POINTER_N) {
                ast_node_t* ptr_node = new_pointer(decl);
                return ptr_node;
            } 
            else if (!decl->function.left || decl->function.left->type == IDENT_N) decl->function.left = base;
            else decl->function.left = combine_nodes(base, decl->function.left);

            return decl;

        case DECLSPEC_N:
            // declaration specifiers should be the base type
            // if base is another DECLSPEC_N or something else, we can combine them

            // combine multiple declaration specifiers
            if (base->type == DECLSPEC_N) return append_item(base, decl);

            // Attach the declspec as the base of the type
            else return combine_nodes(decl, base);
        default:
            return decl;
    }
}
