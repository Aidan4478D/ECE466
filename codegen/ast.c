#include "ast.h"
#include "y.tab.h"
#include "helpers/printing.h"

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


ast_node_t* new_function(char* name, ast_node_t* return_type, ast_node_t* params) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = FUNCT_N;
    new_node->function.name = name;
    new_node->function.return_type = return_type;
    new_node->function.params = params;

    return new_node;
}

ast_node_t* new_function_call(ast_node_t* name, ast_node_t* params) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t)); 

    new_node->type = FUNCTCALL_N;
    new_node->funct_call.name = name;
    new_node->funct_call.params = params;

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
    node->parameter.ident = ident; //extract_ident(ident);  // if size is NULL, then it's an unsized array

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



ast_node_t* new_if(ast_node_t* condition, ast_node_t* then_statement, ast_node_t* else_statement) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = IF_N;

    node->if_node.condition = condition;
    node->if_node.then_statement = then_statement;
    node->if_node.else_statement = else_statement;

    return node;
}

ast_node_t* new_for(ast_node_t* init, ast_node_t* condition, ast_node_t* increment, ast_node_t* body) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = FOR_N;

    node->for_node.init = init;
    node->for_node.condition = condition;
    node->for_node.increment = increment;
    node->for_node.body = body;

    return node;
}

ast_node_t* new_while(NODETYPE type, ast_node_t* condition, ast_node_t* body) {
    ast_node_t* node = (ast_node_t*) malloc(sizeof(ast_node_t));
    node->type = type;

    node->while_node.condition = condition;
    node->while_node.body = body;

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


ast_node_t* new_decl(ast_node_t* specifiers, SYMBOL* declarators) {
    ast_node_t* new_node = (ast_node_t*) malloc(sizeof(ast_node_t));
    new_node->type = DECL_N;

    new_node->decl.specifiers = specifiers;
    new_node->decl.declarators = declarators;

    return new_node;
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
            else if (!decl->function.return_type || decl->function.return_type->type == IDENT_N) decl->function.return_type = base;
            else decl->function.return_type = combine_nodes(base, decl->function.return_type);

            return decl;

        case DECLSPEC_N:
            // declaration specifiers should be the base type
            // if base is another DECLSPEC_N or something else, we can combine them

            // combine multiple declaration specifiers
            if (base->type == DECLSPEC_N) return append_item(base, decl);

            // Attach the declspec as the base of the type
            else return combine_nodes(decl, base);
        case IDENT_N:
            return base;
        default:
            return decl;
    }
}


int compare_types(ast_node_t* type1, ast_node_t* type2) {
    if (type1 == NULL && type2 == NULL) return 1;
    if (type1 == NULL || type2 == NULL) return 0;
    if (type1->type != type2->type) return 0;

    switch (type1->type) {
        case LIST_N:
            return compare_types(type1->list.head, type2->list.head) && compare_types(type1->list.next, type2->list.next);
        case DECLSPEC_N:
            return type1->decl_spec.decl_type == type2->decl_spec.decl_type;
        case POINTER_N:
            return compare_types(type1->pointer.next, type2->pointer.next);
        case ARRAY_N:
            if (type1->array.size != type2->array.size) return 0;
            return compare_types(type1->array.element_type, type2->array.element_type);
        case FUNCT_N:
            return compare_types(type1->function.return_type, type2->function.return_type) && compare_types(type1->function.params, type2->function.params);
        case STRUCT_N:
        case UNION_N:
            return type1->struct_union.sym == type2->struct_union.sym;
        case PARAM_N:
            return compare_types(type1->parameter.type, type2->parameter.type);
        default:
            return 0;
    }
}

void process_declaration(SYMTABLE *cur_scope, SYMBOL *sym, ast_node_t *spec) {
    int stg_class_set = 0;
    ast_node_t* current_spec = spec;
    ast_node_t* type_specs = NULL;

    while (current_spec) {
        ast_node_t* spec_item = current_spec->list.head;

        if (spec_item && spec_item->type == DECLSPEC_N && spec_item->decl_spec.stg_class) {
            if (stg_class_set) {
                fprintf(stderr, "multiple storage class specifiers in declaration\n");
                exit(1);
            }
            //fprintf(stderr, "current spec type is: %s\n", get_node_type(spec_item));
            sym->stg_class = spec_item->decl_spec.stg_class;
            stg_class_set = 1;
            //fprintf(stderr, "Set storage class for %s to: %s\n", sym->key, get_storage_class(sym->stg_class));
        }                                                                    
        if ((spec_item && spec_item->type == DECLSPEC_N && spec_item->decl_spec.decl_type) || spec_item->type == STRUCT_N || spec_item->type == UNION_N) {
            if(!type_specs) type_specs = new_list(spec_item);
            else append_item(type_specs, spec_item);
            //fprintf(stderr, "type spec is of type %s: ", get_node_type(spec_item->type)); 
        }
        current_spec = current_spec->list.next;
    }
    
    // update stack offset if new local variable
    if (sym->type == VAR_SYM && cur_scope->scope != FILE_SCOPE) {
        SYMTABLE* funct_scope = get_enclosing_funct_scope(cur_scope);
        if (funct_scope) {
            fprintf(stderr, "assigning lvar offset!\n");
            sym->stack_offset = funct_scope->lvar_offset;
            funct_scope->lvar_offset -= 4;
        }
    }

    // just keep this as a list of AST nodes for now, it's kinda sketch when printed but it works
    if (type_specs) {
        ast_node_t* base_type = type_specs->list.head;
        sym->node = combine_nodes(base_type, sym->node);
    }

    SYMBOL* found_sym = st_lookup_single(cur_scope, sym->key, sym->name_space);
    int stg_class_match = 0, type_names_match = 0;

    // all these redeclaration rules that I forgot to do in assignment 4
    if(found_sym) {
        stg_class_match = found_sym->stg_class == sym->stg_class ? 1 : 0; 
        type_names_match = compare_types(found_sym->node, sym->node);
        //fprintf(stderr, "found sym '%s' ns is %s, new sym '%s' ns is %s'\n", found_sym->key, get_name_space(found_sym->name_space), sym->key, get_name_space(sym->name_space));
    }

    if(!found_sym) {
        if (st_install(cur_scope, sym) == 0) {
            print_symbol(cur_scope, sym);
            fprintf(stderr, "installed %s into %s with %s ns\n", sym->key, get_scope_name(cur_scope->scope), get_name_space(sym->name_space));
        }
        else fprintf(stderr, "failed to install %s in namespace %s\n", sym->key, get_name_space(sym->name_space));
    }
    else if(found_sym && stg_class_match && type_names_match) fprintf(stderr, "Redefinition of %s %s in %s originally defined <%s>:%d but that's okay! Continuing program...\n", get_symbol_type(sym->type), sym->key, get_scope_name(cur_scope->scope), found_sym->file_name, found_sym->line_num); 
    else {
        fprintf(stderr, "%s '%s' already defined in %s <%s>:%d scope within %s namespace!\n", get_symbol_type(sym->type), sym->key, get_scope_name(cur_scope->scope), cur_scope->start_file, cur_scope->start_line, get_name_space(sym->name_space)); 
        exit(1);
    }
}


ast_node_t* get_pointed_to_type(ast_node_t* node) {
    if (!node) return NULL;
    switch (node->type) {
        case ARRAY_N:
            return node->array.element_type;
        case POINTER_N:
            return node->pointer.next;
        default:
            return NULL;
    }
}

int get_type_size(ast_node_t* type_node) {
    if (!type_node) {
        fprintf(stderr, "NULL type node in get_type_size\n");
        exit(1);
    }
    switch (type_node->type) {
        case DECLSPEC_N:
            switch (type_node->decl_spec.decl_type) {
                case CHAR_DT:    return 1;
                case SHORT_DT:   return 2;
                case INT_DT:     return 4;
                case LONG_DT:    return 4; // long long = 8
                case FLOAT_DT:   return 4;
                case DOUBLE_DT:  return 8; // long double = 12
                case VOID_DT:    return 0;
                default:
                    fprintf(stderr, "unknown decl_type %d in get_type_size\n", type_node->decl_spec.decl_type);
                    exit(1);
            }
        case ARRAY_N:
            if (type_node->array.size == 0) {
                fprintf(stderr, "array size not specified in get_type_size\n");
                exit(1);
            }
            return type_node->array.size * get_type_size(type_node->array.element_type);

        // pointer size 4 in X86-32
        case POINTER_N:
            return 4;
        default:
            fprintf(stderr, "unsupported type %s in get_type_size\n", get_node_type(type_node->type));
            exit(1);
    }
}
