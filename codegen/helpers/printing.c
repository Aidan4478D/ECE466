#include <stdio.h>
#include <string.h>

#include "../ast.h"
#include "../quads.h"
#include "../y.tab.h"

#include "hash.h"
#include "printing.h"
#include "stack.h"

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

int get_binop_opcode(ast_node_t* node) {
	switch(node->genop.op) {
		case '+': return ADD_OC;
		case '-': return SUB_OC;
		case '*': return MUL_OC;
		case '/': return DIV_OC;
		case '%': return MOD_OC;
		default: fprintf(stderr, "binop %d not supprted yet \n", node->genop.op);

	}
	return -1;
}

int get_branch_opcode(int op) {
    switch (op) {
        case '<':   return BRLT_OC;
        case '>':   return BRGT_OC;
        case LTEQ:  return BRLE_OC;
        case GTEQ:  return BRGE_OC;
        case EQEQ:  return BREQ_OC;
        case NOTEQ: return BRNE_OC;

        default: // set to branch opcode on default
            fprintf(stderr, "Unknown comparison operator: %d\n", op);
            return BR_OC;
    }
}

char* get_qnode_type(int op) {
    switch (op) {
        case TEMP_Q:    return "TEMP";
        case VAR_Q:     return "VAR";
        case IMM_Q:     return "IMM";
        case BB_Q:      return "BB";
        case DESC_Q:    return "DESC";
        case STR_Q:     return "STR";

        default: // set to branch opcode on default
            fprintf(stderr, "Unknown qnode type: %d\n", op);
            return NULL;
    }
}

char* print_opcode(int op) {
    switch(op) {
        case LOAD_OC:      return "LOAD";
        case STORE_OC:     return "STORE";
        case LEA_OC:       return "LEA";
        case MOV_OC:       return "MOV";
        case BR_OC:        return "BR";
        case CMP_OC:       return "CMP";
        case BRGT_OC:      return "BRGT";
        case BRLT_OC:      return "BRLT";
        case BRGE_OC:      return "BRGE";
        case BRLE_OC:      return "BRLE";
        case BREQ_OC:      return "BREQ";
        case BRNE_OC:      return "BRNE";
        case ADD_OC:       return "ADD";
        case SUB_OC:       return "SUB";
        case MUL_OC:       return "MUL";
        case DIV_OC:       return "DIV";
        case MOD_OC:       return "MOD";
        /*case AND_OC:       return "AND";*/
        /*case OR_OC:        return "OR";*/
        /*case XOR_OC:       return "XOR";*/
        /*case NOT_OC:       return "NOT";*/
        /*case ANDAND_OC:    return "ANDAND";*/
        /*case OROR_OC:      return "OROR";*/
        /*case LOGNOT_OC:    return "LOGNOT";*/
        /*case SL_OC:        return "SL";*/
        /*case SR_OC:        return "SR";*/
        case CALL_OC:      return "CALL";
        case ARG_OC:       return "ARG";
        case RETURN_OC:    return "RETURN";
		default: fprintf(stderr, "opcode %d not found\n", op);
    }

    return NULL;
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
        case SIZEOF:   return "sizeof";
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

// below, all the AST and symbol printing is pretty inefficient but it works
// I know it could be optimized but to be honest I just can't bother to do it

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
            printf("FUNCTION '%s'\n", node->function.name);
            if (node->function.return_type) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("RETURN TYPE: ");
                if (node->function.return_type->type == POINTER_N && node->function.return_type->pointer.next && node->function.return_type->pointer.next->type == FUNCT_N) {
                    printf("POINTER to\n");
                    print_full_type(node->function.return_type->pointer.next, indent + 2);
                } else {
                    print_type(node->function.return_type);
                    printf("\n");
                }
            }
            if (node->function.params) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("PARAMETERS:\n");
                ast_node_t* param_list = node->function.params;
                int param_count = 1;
                while (param_list) {
                    for (int i = 0; i < indent + 2; i++) printf("\t");
                    printf("PARAMETER #%d\n", param_count++);
                    print_ast_tree(param_list->list.head, indent + 3);
                    param_list = param_list->list.next;
                }
            }
            break;

        case DECL_N:
            // already indented from 
            printf("DECLARATION\n");

            // decl specs
            if (node->decl.specifiers) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("SPECIFIERS: ");
                if (node->decl.specifiers->type == LIST_N) {
                    ast_node_t* current = node->decl.specifiers;
                    while (current) {
                        if (current->list.head->type == DECLSPEC_N) printf("%s ", get_decl_spec(current->list.head->decl_spec.decl_type));
                        else print_type(current->list.head);
                        current = current->list.next;
                    }
                } 
                else if (node->decl.specifiers->type == DECLSPEC_N) printf("%s ", get_decl_spec(node->decl.specifiers->decl_spec.decl_type));
                else print_type(node->decl.specifiers);
                printf("\n");
            }

            // declarators
            if (node->decl.declarators) {
                SYMBOL* sym = node->decl.declarators;
                int decl_count = 1;
                while (sym) {
                    for (int i = 0; i < indent + 1; i++) printf("\t");
                    printf("DECLARATOR #%d\n", decl_count++);
                    for (int i = 0; i < indent + 2; i++) printf("\t");
                    printf("IDENT: %s\n", sym->key); 

                    // could be redundant but could also not be!
                    if (sym->node) {
                        for (int i = 0; i < indent + 2; i++) printf("\t");
                        printf("TYPE: ");
                        print_type(sym->node);
                        printf("\n");
                    }
                    sym = sym->next;
                }
            }
            break;
        case SWITCH_N:
            printf("SWITCH\n");
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("EXPRESSION:\n");
            print_ast_tree(node->switch_node.expression, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("STATEMENT: (is a list, I just got rid of the words \"list\" and \"list element\" to look nice)\n");
            if (node->switch_node.statement) {
                if (node->switch_node.statement->type == LIST_N) {
                    ast_node_t* stmt_list = node->switch_node.statement;
                    while (stmt_list) {
                        print_ast_tree(stmt_list->list.head, indent + 2);
                        stmt_list = stmt_list->list.next;
                    }
                }
                else print_ast_tree(node->switch_node.statement, indent + 2);
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
            /*if (node->unop.op == SIZEOF) {*/
                /*for (int i = 0; i < indent + 1; i++) printf("\t");*/
                /*if (node->unop.node->type == DECLSPEC_N || node->unop.node->type == POINTER_N || node->unop.node->type == ARRAY_N || node->unop.node->type == IDENT_N) {*/
                    /*printf("SIZEOF type:\n");*/
                /*} */
                /*else printf("SIZEOF expression:\n");*/
            /*}*/
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
            for (int i = 0; i < indent; i++) printf("\t");
            printf("THEN:\n");
            print_ast_tree(node->ternop.center, indent + 1);
            for (int i = 0; i < indent; i++) printf("\t");
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
            print_full_type(node, indent); 
            printf("\n"); 
            break;
        case FUNCTCALL_N:
            printf("FUNCTION CALL\n");
            if (node->funct_call.name) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                print_type(node->funct_call.name);
            }
            if (node->funct_call.params) {
                ast_node_t* param_list = node->funct_call.params;
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
                    printf("\n"); 
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
        case IF_N:
            printf("IF\n");
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("COND:\n");
            print_ast_tree(node->if_node.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("THEN:\n");
            print_ast_tree(node->if_node.then_statement, indent + 2);
            if (node->if_node.else_statement) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("ELSE:\n");
                print_ast_tree(node->if_node.else_statement, indent + 2);
            }
            break;
        case WHILE_N:
            printf("WHILE\n");
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("COND:\n");
            print_ast_tree(node->while_node.condition, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("BODY:\n");
            print_ast_tree(node->while_node.body, indent + 2);
            break;
        case DOWHILE_N:
            printf("DO_WHILE\n");
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("BODY:\n");
            print_ast_tree(node->while_node.body, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("COND:\n");
            print_ast_tree(node->while_node.condition, indent + 2);
            break;
        case FOR_N:
            printf("FOR%s\n", !node->for_node.init && !node->for_node.condition && !node->for_node.increment ? "(ever)" : "");
            if (node->for_node.init) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("INIT:\n");
                print_ast_tree(node->for_node.init, indent + 2);
            }
            if (node->for_node.condition) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("COND:\n");
                print_ast_tree(node->for_node.condition, indent + 2);
            }
            if (node->for_node.increment) {
                for (int i = 0; i < indent + 1; i++) printf("\t");
                printf("INCR:\n");
                print_ast_tree(node->for_node.increment, indent + 2);
            }
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("BODY:\n");
            print_ast_tree(node->for_node.body, indent + 2);
            break;
        case SWITCH_N:
            print_full_type(node, indent);
            printf("\n"); 
            break;
        case RETURN_N:
            printf("RETURN\n");
            if (node->return_node.expression) print_ast_tree(node->return_node.expression, indent + 1);
            break;
        case GOTO_N:
            printf("GOTO %s (%s) defined at <%s>:%d\n", node->goto_node.sym->key, node->goto_node.sym->is_complete == 1 ? "complete" : "incomplete", node->goto_node.sym->file_name, node->goto_node.sym->line_num);
            break;
        case CONTINUE_N:
            printf("CONTINUE\n");
            break;
        case BREAK_N:
            printf("BREAK\n");
            break;
        case LABEL_N:
            printf("LABEL (%s) defined at <%s>:%d\n", node->label_node.sym->key, node->label_node.sym->file_name, node->label_node.sym->line_num);
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("STATEMENT:\n");
            print_ast_tree(node->label_node.statement, indent + 2);
            break;
        case CASE_N:
            printf("CASE\n");
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("EXPRESSION:\n");
            print_ast_tree(node->switch_label.name, indent + 2);
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("STATEMENT:\n");
            print_ast_tree(node->switch_label.statement, indent + 2);
            break;
        case DEFAULT_N:
            printf("DEFAULT\n");
            for (int i = 0; i < indent + 1; i++) printf("\t");
            printf("STATEMENT:\n");
            print_ast_tree(node->switch_label.statement, indent + 2);
            break;
        case DECL_N:
            print_full_type(node, indent);
            printf("\n"); 
            break;
        default:
            printf("UNKNOWN NODE TYPE %d\n", node->type);
            break;
    }
}
