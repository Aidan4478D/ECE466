#include <stdio.h>
#include "hash.h"
#include "y.tab.h"
#include "helpers.h"
#include "stack.h"
#include <string.h>

void print_current_scope(stack_t* scope_stack) {
    SYMTABLE *st = (SYMTABLE*) stack_peek(scope_stack);
    printf("Current scope: %d\n", st->scope);
}


void print_sym_table(SYMTABLE *st) {
    if (!st || !st->ht) {
        printf("Symbol table is empty.\n");
        return;
    }
    
    ht_t *ht = st->ht;
    /*printf("Symbol Table (Scope: %s, Capacity: %d, Filled: %d):\n", get_scope_name(st->scope), ht->capacity, ht->filled);*/
    
    printf("\n---------------------------------------------\n");
    for (int i = 0; i < ht->capacity; i++) {
        hash_item *item = &ht->data[i];
        if (item->isOccupied && !item->isDeleted) {
            SYMBOL *sym = (SYMBOL *) item->pv;
            printf("<%s>:%d\n", sym->file_name, sym->line_num);
            printf("symbol: %s, scope: %s, namespace = %s, type = %s, storage = %s\n", sym->key, get_scope_name(st->scope), get_name_space(sym->name_space), get_symbol_type(sym->type), get_storage_class(sym->stg_class));
            //printf("AST Tree for your symbol:\n---------------------------------------------\n");
            //print_ast_tree(sym->node, 0);
        }
    }
    printf("---------------------------------------------\n");
}


void print_symbol(SYMTABLE *st, SYMBOL* sym) {
    if (!sym) {
        printf("Symbol is NULL.\n");
        return;
    }
    if (st_lookup(st, sym->key, sym->name_space)) {
        printf("---------------------------------------------\n");
        if (sym->type == MEMBER_SYM && sym->parent_sym) {
            const char* su_type = (sym->parent_sym->type == STRUCT_SYM) ? "struct" : "union";
            printf("[in %s %s starting at <%s>:%d]\n", su_type, sym->parent_sym->key ? sym->parent_sym->key : "(anonymous)", sym->parent_sym->file_name, sym->parent_sym->line_num);
        } 
        else if (sym->scope) printf("[in %s scope starting at <%s>:%d]\n", get_scope_name(st->scope), st->start_file, st->start_line);

        printf("<%s>:%d\n", sym->file_name, sym->line_num);
        printf("symbol: %s, scope: %s, namespace = %s, type = %s, storage = %s\n", sym->key, get_scope_name(st->scope), get_name_space(sym->name_space), get_symbol_type(sym->type), get_storage_class(sym->stg_class));
        printf("AST Tree for your symbol:\n---------------------------------------------\n");

        if (sym->type == FUNCT_SYM) print_full_type(sym->node, 0); 
        else if (sym->type == VAR_SYM || sym->type == MEMBER_SYM) {
            printf("TYPE: ");
            print_type(sym->node);
            printf("\n");
        } 
        else print_ast_tree(sym->node, 0);
    
        printf("---------------------------------------------\n\n");
    } 
    else fprintf(stderr, "Symbol %s not found in %s symbol table!\n", sym->key, get_scope_name(st->scope));
    
}

char* get_operator_string(int op) {
    switch(op) {
        case EQEQ:     return "==";
        case NOTEQ:    return "!=";
        case LTEQ:     return "<=";
        case GTEQ:     return ">=";
        case PLUSEQ:   return "+=";
        case MINUSEQ:  return "-=";
        case MULTEQ:   return "*=";
        case DIVEQ:    return "/=";
        case MODEQ:    return "%=";
        case SLEQ:     return "<<=";
        case SREQ:     return ">>=";
        case ANDEQ:    return "&=";
        case XOREQ:    return "^=";
        case OREQ:     return "|=";
        case ANDAND:   return "&&";
        case OROR:     return "||";
        case SL:       return "<<";
        case SR:       return ">>";
        case PLUSPLUS: return "++";
        case MINMIN:   return "--";
        default: {
            /* For any other operator (or if op is a simple char) */
            static char buf[3];
            snprintf(buf, sizeof(buf), "%c", op);
            return buf;
        }
    }
}

char* get_scope_name(int scope) {
    switch (scope) {
        case FILE_SCOPE: return "FILE_SCOPE";
        case FUNCT_SCOPE: return "FUNCT_SCOPE";
        case BLOCK_SCOPE: return "BLOCK_SCOPE";
        case PROTO_SCOPE: return "PROTO_SCOPE"; 
        default: return "UNKNOWN_SCOPE";
    }
}

char* get_decl_spec(int op) {
    switch(op) {
        case FLOAT_DT:      return "FLOAT";
        case DOUBLE_DT:     return "DOUBLE";
        case INT_DT:        return "INT";
        case LONG_DT:       return "LONG";
        case SHORT_DT:      return "SHORT";
        case CHAR_DT:       return "CHAR";
        case VOID_DT:       return "VOID";
        case SIGNED_DT:     return "SIGNED";
        case UNSIGNED_DT:   return "UNSIGNED";
        default: {
            static char buf[3];
            snprintf(buf, sizeof(buf), "%c", op);
            return buf;
        }
    }
}

char* get_storage_class(int op) {
    switch(op) {
        case REG_SC:        return "REGISTER";
        case AUTO_SC:       return "AUTO";
        case EXTERN_SC:     return "EXTERN";
        case STATIC_SC:     return "STATIC";
        case UNKNOWN_SC:    return "UNKNOWN";
        default: {
            static char buf[3];
            snprintf(buf, sizeof(buf), "%c", op);
            return buf;
        }
    }
}


char* get_name_space(int op) {
    switch(op) {
        case TAG_NS:        return "TAG";
        case LABEL_NS:      return "LABEL";
        case MEMBER_NS:     return "MEMBER";
        case GENERAL_NS:    return "GENERAL";
        default: {
            static char buf[3];
            snprintf(buf, sizeof(buf), "%c", op);
            return buf;
        }
    }
}


char* get_symbol_type(int op) {
    switch(op) {
        case VAR_SYM:       return "VARIABLE";
        case FUNCT_SYM:     return "FUNCTION";
        case STRUCT_SYM:    return "STRUCT";
        case UNION_SYM:     return "UNION";
        case MEMBER_SYM:    return "MEMBER";
        case LABEL_SYM:     return "LABEL";
        default: {
            static char buf[3];
            snprintf(buf, sizeof(buf), "%c", op);
            return buf;
        }
    }
}


char* get_node_type(int op) {
    switch(op) {
        case IDENT_N:       return "IDENT";
        case STRING_N:      return "STRING";
        case NUMBER_N:      return "NUMBER";
        case POSTFIX_N:     return "POSTFIX OP";
        case UNOP_N:        return "UNARY OP";
        case BINOP_N:       return "BINARY OP";
        case TERNOP_N:      return "TERNARY OP";
        case ASSIGNOP_N:    return "ASSIGNMENT OP";
        case COMPOP_N:      return "COMPARISON OP";
        case FUNCT_N:       return "FUNCTION";
        case LIST_N:        return "LIST";
        case ELEMENT_N:     return "ELEMENT";
        case LOGOP_N:       return "LOGICAL OP";
        case POINTER_N:     return "POINTER";
        case ARRAY_N:       return "ARRAY";
        case DECLSPEC_N:    return "DECLARATION SPECIFIER";
        case PARAM_N:       return "PARAMETER";
        case STRUCT_N:      return "STRUCT";
        case UNION_N:       return "UNION";
        default: {
            static char buf[3];
            snprintf(buf, sizeof(buf), "%c", op);
            return buf;
        }
    }
}

// below is pretty inefficient but it works

// use for making display look nice in output AST tree
void print_type(ast_node_t *node) {
    if (!node) return;

    switch (node->type) {
        case LIST_N: {
            ast_node_t* current = node;
            while (current) {
                print_ast_tree(current->list.head, 0);
                if (current->list.next) printf(" ");
                current = current->list.next;
            }
            break;
        }
        case POINTER_N:
            printf("POINTER to ");
            print_type(node->pointer.next);
            break;
        case ARRAY_N:
            printf("ARRAY of size %d of ", node->array.size);
            print_type(node->array.element_type);
            break;
        case STRUCT_N:
            printf("STRUCT %s ", node->struct_union.sym->key ? node->struct_union.sym->key : "(anonymous)");
            printf("(%s) ", node->struct_union.sym->is_complete ? "complete" : "incomplete");
            if(node->struct_union.sym->parent_sym) printf("defined at <%s>:%d\n", node->struct_union.sym->parent_sym->file_name, node->struct_union.sym->parent_sym->line_num);
            else printf("\n");
            break;
        case UNION_N:
            printf("UNION %s\n", node->struct_union.sym->key ? node->struct_union.sym->key : "(anonymous)");
            printf(" (%s)\n", node->struct_union.sym->is_complete ? "complete" : "incomplete");
            if(node->struct_union.sym->parent_sym) printf("defined at <%s>:%d\n", node->struct_union.sym->parent_sym->file_name, node->struct_union.sym->parent_sym->line_num);
            else printf("\n");
            break;
        default:
            print_ast_tree(node, 0);
            break;
    }
}



void print_full_type(ast_node_t* node, int indent) {
    if (!node) {
        for (int i = 0; i < indent; i++) printf("\t");
        printf("unknown\n");
        return;
    }

    switch (node->type) {
        case FUNCT_N:
            for (int i = 0; i < indent; i++) printf("\t");
            printf("FUNCTION CALL\n");
            if (node->function.left) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("RETURN TYPE: ");
                if (node->function.left->type == POINTER_N && 
                    node->function.left->pointer.next && 
                    node->function.left->pointer.next->type == FUNCT_N) {
                    printf("POINTER to\n");
                    print_full_type(node->function.left->pointer.next, indent + 2);
                } else {
                    print_type(node->function.left);
                    printf("\n");
                }
            }
            if (node->function.right) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("PARAMETERS:\n");
                ast_node_t* param_list = node->function.right;
                int param_count = 1;
                while (param_list) {
                    for (int i = 0; i < indent + 2; i++) printf("\t");
                    printf("PARAMETER #%d\n", param_count++);
                    print_ast_tree(param_list->list.head, indent + 3);
                    param_list = param_list->list.next;
                }
            }
            break;
        case DECLSPEC_N:
            for (int i = 0; i < indent; i++) printf("\t");
            printf("%s\n", get_decl_spec(node->decl_spec.decl_type));
            break;
        default:
            print_ast_tree(node, indent);
            break;
    }
}



void print_ast_tree(ast_node_t *node, int indent) {
    if (!node)
        return;
    
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }

    switch(node->type) {
        case IDENT_N:
            printf("IDENT: %s\n", node->ident.name);
            break;
        case STRING_N:
            if(node->string.str_meta.type == STRING_T) printf("STRING %s\n", node->string.str_meta.string_literal);
            if(node->string.str_meta.type == CHAR_T) printf("CHAR %c\n", node->string.str_meta.char_literal);
            break;
        case NUMBER_N:
            switch (node->number.num_meta.type) {
                case INT_T:     
                    if(node->number.num_meta.sign == SIGNED_T) printf("NUM (numtype=int)\t%lld\tSIGNED\n", node->number.num_meta._int);
                    if(node->number.num_meta.sign == UNSIGNED_T) printf("NUM (numtype=int)\t%llu\tUNSIGNED\n", node->number.num_meta._int);
                    break;
                case LONG_T:
                    if(node->number.num_meta.sign == SIGNED_T) printf("NUM (numtype=long)\t%lld\tSIGNED\n", node->number.num_meta._int);
                    if(node->number.num_meta.sign == UNSIGNED_T) printf("NUM (numtype=long)\t%llu\tUNSIGNED\n", node->number.num_meta._int);
                    break;
                case LONGLONG_T:
                    if(node->number.num_meta.sign == SIGNED_T) printf("NUM (numtype=longlong)\t%lld\tSIGNED\n", node->number.num_meta._int);
                    if(node->number.num_meta.sign == UNSIGNED_T) printf("NUM (numtype=longlong)\t%llu\tUNSIGNED\n", node->number.num_meta._int);
                    break;
                case FLOAT_T:
                    if(node->number.num_meta.sign == SIGNED_T) printf("NUM (numtype=float)\t%f\tSIGNED\n", node->number.num_meta._float);
                    break;
                case DOUBLE_T:
                    if(node->number.num_meta.sign == SIGNED_T) printf("NUM (numtype=double)\t%Lf\tSIGNED\n", node->number.num_meta._double);
                    break;
                case LONGDOUBLE_T:
                    if(node->number.num_meta.sign == SIGNED_T) printf("NUM (numtype=longdouble)\t%Lf\tSIGNED\n", node->number.num_meta._double);
                    break;
                default:
                    printf("UNKNOWN NUM TYPE %d\n", node->number.num_meta.type);
                    break;
            }
            break;
        case UNOP_N: {
            const char *op_str = get_operator_string(node->unop.op);
            printf("UNARY OP (%s)\n", op_str);
            if (node->unop.op == SIZEOF) {
                if (node->unop.node->type == DECLSPEC_N || node->unop.node->type == POINTER_N || node->unop.node->type == ARRAY_N) {
                    printf("sizeof type:\n");
                } 
                else printf("sizeof expression:\n");
            }
            print_ast_tree(node->unop.node, indent + 1);
            break;
        }
        case BINOP_N: {
            const char *op_str = get_operator_string(node->genop.op);
            printf("BINARY OP (%s)\n", op_str);
            print_ast_tree(node->genop.left, indent + 1);
            print_ast_tree(node->genop.right, indent + 1);
            break;
        }
        case TERNOP_N:
            printf("TERNARY OP, IF:\n");
            print_ast_tree(node->ternop.left, indent + 1);
            printf("THEN:\n");
            print_ast_tree(node->ternop.center, indent + 1);
            printf("ELSE:\n");
            print_ast_tree(node->ternop.right, indent + 1);
            break;
        case ASSIGNOP_N: {
            const char *op_str = get_operator_string(node->genop.op);
            printf("ASSIGNMENT OP (%s)\n", op_str);
            print_ast_tree(node->genop.left, indent + 1);
            print_ast_tree(node->genop.right, indent + 1);
            break;
        }
        case COMPOP_N: {
            const char *op_str = get_operator_string(node->genop.op);
            printf("COMPARISON OP (%s)\n", op_str);
            print_ast_tree(node->genop.left, indent + 1);
            print_ast_tree(node->genop.right, indent + 1);
            break;
        }
        case FUNCT_N:
            printf("FUNCTION CALL\n");
            if (node->function.left) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("RETURN TYPE: ");
                print_type(node->function.left);
                printf("\n");
            }
            if (node->function.right) {
                ast_node_t* param_list = node->function.right;
                int param_count = 1;
                while (param_list) {
                    for (int i = 0; i < indent + 1; i++) printf("\t");
                    printf("PARAMETER #%d\n", param_count++);
                    print_ast_tree(param_list->list.head, indent + 2);
                    param_list = param_list->list.next;
                }
            }
            break;
        case LOGOP_N: {
            const char *op_str = get_operator_string(node->genop.op);
            printf("LOGICAL OP (%s)\n", op_str);
            print_ast_tree(node->genop.left, indent + 1);
            print_ast_tree(node->genop.right, indent + 1);
            break;
        }
        case ELEMENT_N:
            printf("ELEMENT:\n");
            print_ast_tree(node->list.head, indent + 1);
            break;
        case LIST_N:
            printf("LIST:\n");
            {
                ast_node_t* current = node;
                int counter = 1;
                while (current) {
                    for (int i = 0; i < indent; i++) printf("\t");
                    printf("LIST ELEMENT #%d:\n", counter++);
                    print_ast_tree(current->list.head, indent + 1);
                    current = current->list.next;
                }
            }
            break;
        case POINTER_N:
            printf("POINTER to:\n");
            print_ast_tree(node->pointer.next, indent + 1);
            break;
        case ARRAY_N: 
            printf("ARRAY of size: %d\n", (node->array.size ? node->array.size : 0));
            print_ast_tree(node->array.element_type, indent + 1);
            break;
        case DECLSPEC_N:
            if (node->decl_spec.stg_class) printf("STORAGE CLASS: %s\n", get_storage_class(node->decl_spec.stg_class));
            if (node->decl_spec.decl_type) printf("%s", get_decl_spec(node->decl_spec.decl_type));
            break;
        case PARAM_N:
            if (node->parameter.ident) {
                // extract ident in ast.c
                ast_node_t* ident_node = extract_ident(node->parameter.ident);
                if (ident_node) printf("IDENT: %s\n", ident_node->ident.name);
            }
            if (node->parameter.type) {
                if(node->parameter.ident) for (int i = 0; i < indent; i++) printf("\t");
                printf("TYPE: ");
                print_type(node->parameter.type);
                printf("\n");
            }
            break;
        case STRUCT_N:
            printf("STRUCT %s ", node->struct_union.sym->key ? node->struct_union.sym->key : "(anonymous)");
            printf("(%s) ", node->struct_union.sym->is_complete ? "complete" : "incomplete");
            if(node->struct_union.sym->parent_sym) printf("defined at <%s>:%d\n", node->struct_union.sym->parent_sym->file_name, node->struct_union.sym->parent_sym->line_num);
            else printf("\n");
            break;
        case UNION_N:
            printf("UNION %s\n", node->struct_union.sym->key ? node->struct_union.sym->key : "(anonymous)");
            printf(" (%s)\n", node->struct_union.sym->is_complete ? "complete" : "incomplete");
            if(node->struct_union.sym->parent_sym) printf("defined at <%s>:%d\n", node->struct_union.sym->parent_sym->file_name, node->struct_union.sym->parent_sym->line_num);
            else printf("\n");
            break;
        default:
            printf("UNKNOWN NODE TYPE %d\n", node->type);
            break;
    }
}
