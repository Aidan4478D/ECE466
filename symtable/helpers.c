#include <stdio.h>
#include "hash.h"
#include "y.tab.h"
#include "helpers.h"

void print_sym_table(SYMTABLE *st) {
    if (!st || !st->ht) {
        printf("Symbol table is empty.\n");
        return;
    }
    
    ht_t *ht = st->ht;
    printf("Symbol Table (Scope: %d, Capacity: %d, Filled: %d):\n", st->scope, ht->capacity, ht->filled);
    
    for (int i = 0; i < ht->capacity; i++) {
        hash_item *item = &ht->data[i];
        if (item->isOccupied && !item->isDeleted) {
            SYMBOL *sym = (SYMBOL *) item->pv;
            // For clarity, you might later convert enum values to strings.
            printf("Slot %d: key = %s, namespace = %d, type = %d, storage = %d\n",
                   i, sym->key, sym->lineno, sym->name_space, sym->type, sym->stg_class);
        }
    }
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


void print_ast_tree(ast_node_t *node, int indent) {
    if (!node)
        return;
    
    for (int i = 0; i < indent; i++) {
        printf("\t");
    }

    switch(node->type) {
        case IDENT_N:
            printf("IDENT %s\n", node->ident.name);
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
            print_ast_tree(node->function.left, indent + 1);
            if (node->function.right)
                print_ast_tree(node->function.right, indent + 1);
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
            printf("LIST:\n"); {
                ast_node_t* current = node;
                int counter = 1;

                while (current) {
                    for (int i = 0; i < indent; i++)
                        printf("\t");

                    printf("LIST ELEMENT #%d:\n", counter++); 
                    print_ast_tree(current->list.head, indent + 1);
                    current = current->list.next;
                }
            }
            break;
        default:
            printf("UNKNOWN NODE TYPE %d\n", node->type);
            break;
    }
}

