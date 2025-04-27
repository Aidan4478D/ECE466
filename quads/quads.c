#include <string.h>

#include "quads.h"
#include "helpers/printing.h"
#include "symtable.h"

int funct_count = 0;
int bb_count = 0;
int tmp_count = 0;
BASICBLOCK* cur_bb;

BASICBLOCK* create_quads(ast_node_t* listnode) {

    // create linked list for quads
    BASICBLOCK* bb = new_bb();
    cur_bb = bb;

    while(listnode) {
        ast_node_t* node = listnode->list.head;
        if(node) create_statement(node);
        listnode = listnode->list.next;
    }
    print_all(bb);
    funct_count++; 
        
    return bb;
}

void print_all(BASICBLOCK* bb) {

    BASICBLOCK* cur = bb;
    while(cur) {
        printf(".BB%d.%d \n", cur->funct_num, cur->bb_num);
        print_bb(cur);
        cur = cur->next;
    }
    fprintf(stderr, "Finished printing all BBs\n"); 
}


// print all quads in a basic block
void print_bb(BASICBLOCK* bb) {
    if (!bb || !bb->quad_list) {
        printf("Empty basic block\n");
        return;
    }
    list_t* list = bb->quad_list;
    list_node_t* current = list->head;
    while (current != NULL) {
        QUAD* quad = (QUAD*) current->data;
        print_quad(quad);
        current = current->next;
    }
}

// Print a single quad
void print_quad(QUAD* quad) {
    if (!quad) {
        printf("NULL_QUAD\n");
        return;
    }
    if (quad->destination) {
        printf("\t");
        print_qnode(quad->destination);
        if (quad->src1 || quad->src2) printf(" = ");
    }
    printf("%s \t", print_opcode(quad->oc));
    if (quad->src1) {
        print_qnode(quad->src1);
        if (quad->src2) printf(", ");
    }
    if (quad->src2) {
        print_qnode(quad->src2);
    }
    printf("\n");
}


// print a QNODE (temporary or variable)
void print_qnode(QNODE* qnode) {
    if (!qnode) {
        printf("NULL");
        return;
    }
    switch(qnode->type) {
        case TEMP_Q:
            printf("T%d", qnode->tmp_id);
            break;
        case IMM_Q: {
            if(qnode->ast_node->type == NUMBER_N) {
                ast_node_t* node = qnode->ast_node;
                switch (node->number.num_meta.type) {
                    case INT_T:     
                        if(node->number.num_meta.sign == SIGNED_T) printf("%lld{S}", node->number.num_meta._int);
                        if(node->number.num_meta.sign == UNSIGNED_T) printf("%llu{U}", node->number.num_meta._int);
                        break;
                    case LONG_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("%lld{S}", node->number.num_meta._int);
                        if(node->number.num_meta.sign == UNSIGNED_T) printf("%llu{U}", node->number.num_meta._int);
                        break;
                    case LONGLONG_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("%lld{S}", node->number.num_meta._int);
                        if(node->number.num_meta.sign == UNSIGNED_T) printf("%llu{U}", node->number.num_meta._int);
                        break;
                    case FLOAT_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("%f\n", node->number.num_meta._float);
                        break;
                    case DOUBLE_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("%Lf\n", node->number.num_meta._double);
                        break;
                    case LONGDOUBLE_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("%Lf\n", node->number.num_meta._double);
                        break;
                    default:
                        printf("UNKNOWN NUM TYPE %d\n", node->number.num_meta.type);
                        break;
                }

            }
            break;
        }
        case VAR_Q:
            if (qnode->ast_node && qnode->ast_node->type == IDENT_N) {
                fprintf(stderr, "qnode %s: %s\n", qnode->ast_node->ident.name, get_scope_name(qnode->scope)); 
                printf("%s{%s}", qnode->ast_node->ident.name, (qnode->scope == FILE_SCOPE ? "global" : "lvar"));
            } 
            else printf("VAR_UNKNOWN");
            break;

        default: printf("UNKNOWN_QNODE");
    }
}



QUAD* create_statement(ast_node_t* node) {

    QUAD* quad_node = (QNODE*) malloc(sizeof(QNODE));

    // assign what the quad node should be here
    switch(node->type) {

        case BINOP_N: //binop within genop
            fprintf(stderr, "BINARY OP detected!\n");
            // this should be handled in gen assignment
            
            //*QNODE left = create_rvalue(node->left, NULL);
            //*QNODE right = create_rvalue(node->right, NULL);
            // return emit(get_binop_opcode(node->genop.op), node->genop.left, node->genop.right, NULL);
        case UNOP_N:
            fprintf(stderr, "UNARY OP detected!\n"); 
            break;
        case ASSIGNOP_N:
            fprintf(stderr, "ASSIGN OP detected!\n"); 
            return create_assignment(node);
        case FUNCT_N:
            fprintf(stderr, "FUNCT detected!\n"); 
            break;
        case FUNCTCALL_N:
            fprintf(stderr, "FUNCT CALL detected!\n"); 
            break;
        case IF_N:
            fprintf(stderr, "IF detected!\n");
            create_if(node);
            break;
        case WHILE_N:
            fprintf(stderr, "WHILE detected!\n"); 
            break;
        case DOWHILE_N:
            fprintf(stderr, "DO WHILE detected!\n"); 
            break;
        case FOR_N:
            fprintf(stderr, "FOR detected!\n"); 
            break;
        case BREAK_N:
            fprintf(stderr, "BREAK detected!\n"); 
            break;
        case CONTINUE_N:
            fprintf(stderr, "CONTINUE detected!\n"); 
            break;
        case RETURN_N:
            fprintf(stderr, "RETURN detected!\n");

            if (node->return_node.expression) {
                QNODE* ret_val = create_rvalue(node->return_node.expression, NULL);
                emit(RETURN_OC, ret_val, NULL, NULL);
            } 
            else emit(RETURN_OC, NULL, NULL, NULL);
            
            break;
        case DECL_N:
            // don't generate quads for declarations
            return NULL;
        default:
            fprintf(stderr, "invalid statement type: node_type = %s (%d)\n", get_node_type(node->type), node->type);
            break;
    }

    return quad_node;
}


// target being some register or address
QNODE* create_rvalue(ast_node_t* node, QNODE* target) {

    QNODE *left, *right;
    switch(node->type) {
        case DECLSPEC_N:
            break;
        case NUMBER_N: { // new immediate value
            QNODE* qnode = (QNODE*) malloc(sizeof(QNODE));

            qnode->type = IMM_Q;
            qnode->ast_node = node;
            
            return qnode;
        }
        case STRING_N:
            break;
        case IDENT_N: {
            QNODE* qnode = (QNODE*) malloc(sizeof(QNODE));
            SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
            SYMBOL* sym = st_lookup(st, node->ident.name, GENERAL_NS);

            fprintf(stderr, "cur scope is %s for sym %s\n", get_scope_name(st->scope), node->ident.name); 

            if(sym) {
                qnode->sym = sym;
                qnode->scope = sym->scope->scope; //symbol's st's scope
            }
            else fprintf(stderr, "Undefined variable %s\n", node->ident.name); 
            
            qnode->type = VAR_Q;
            qnode->ast_node = node;
            return qnode;
        }
        case BINOP_N:
            fprintf(stderr, "creating bin op for r value\n"); 
            left = create_rvalue(node->genop.left, NULL);
            right = create_rvalue(node->genop.right, NULL);
            if(!target) target = new_temporary();
            emit(get_binop_opcode(node), left, right, target); 
            return target;
        case POINTER_N:
            QNODE* addr = create_rvalue(node->pointer.next, NULL);
            if(!target) target = new_temporary();
            emit(LOAD_OC, addr, NULL, target);
            return target;
        default:
            fprintf(stderr, "invalid r-value type: %s\n", get_node_type(node->type));
            return NULL;
    }
    return NULL;
}


QNODE* create_lvalue(ast_node_t* node, int* mode) {

    switch(node->type) {
        case IDENT_N:
            *mode = DIRECT_MODE;

            QNODE* qnode = (QNODE*) malloc(sizeof(QNODE));
            qnode->type = VAR_Q;
            qnode->ast_node = node;
            
            SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
            SYMBOL* sym = st_lookup(st, node->ident.name, GENERAL_NS);

            if (sym) {
                qnode->sym = sym;
                qnode->scope = sym->scope->scope;  // Set to the symbol's scope
            } 
            else fprintf(stderr, "Undefined variable %s\n", node->ident.name);

            return qnode;
        case NUMBER_N: return NULL;
        case UNOP_N:
            // pointer dereference
            if(node->unop.op == '*') {
                *mode = INDIRECT_MODE;
                return create_rvalue(node->unop.node, NULL); 
            }
        default:
            fprintf(stderr, "invalid r-value type: %s\n", get_node_type(node->type));
            return NULL;
    }
}


QNODE* create_assignment(ast_node_t* node) {

    if(node->type == ASSIGNOP_N) {
        int destmode;
        QNODE* dst = create_lvalue(node->genop.left, &destmode);

        if(destmode == DIRECT_MODE) {
            QNODE* tmp = create_rvalue(node->genop.right, dst);
            // will get emitted in create rvalue
        }
        else {
            QNODE* t1 = create_rvalue(node->genop.right, NULL);
            emit(STORE_OC, t1, dst, NULL); 
        }
    }
    /*fprintf(stderr, "Create assignemnt called with node type: %s\n", get_node_type(node->type));*/
    return NULL;
}


void create_condexpr(ast_node_t* expr, BASICBLOCK* Bt, BASICBLOCK* Bf) {

    /*switch(expr->type) {*/
        /*case IF_N: */
        /*case WHILE_N:*/
        /*case DOWHILE_N:*/
        
    /*}*/
}


void create_if(ast_node_t* node) {

    BASICBLOCK* Bt = new_bb();
    BASICBLOCK* Bf = new_bb();
    BASICBLOCK* Bn;

    if(node->if_node.else_statement) Bn = new_bb();
    else Bn = Bf;

    create_condexpr(node->if_node.condition, Bt, Bf);
    cur_bb = Bt;

    create_statement(node->if_node.then_statement);
    link_bb(cur_bb, ALWAYS_MODE, Bn, NULL);

    if(node->if_node.else_statement) {
        cur_bb = Bf;
        create_statement(node->if_node.else_statement);
        link_bb(cur_bb, ALWAYS_MODE, Bn, NULL);
    }
    cur_bb = Bn;
}

void link_bb(BASICBLOCK* cur_bb, MODE mode, BASICBLOCK* Bt, BASICBLOCK* Bf) {


}


// create new node of type TEMPORARY
QNODE* new_temporary() {
 
    QNODE* node = (QNODE*) malloc(sizeof(QNODE));
    
    node->type = TEMP_Q;
    node->tmp_id = tmp_count++;

    node->scope = -1;
    node->sym = NULL;
    node->ast_node = NULL;
    
    return node;
}

QUAD* emit(OPCODE oc, QNODE* src1, QNODE* src2, QNODE* destination) {

    QUAD* quad = (QUAD*) malloc(sizeof(QUAD));

    quad->oc = oc;
    quad->src1 = src1;
    quad->src2 = src2;
    quad->destination = destination;

    list_insert_tail(cur_bb->quad_list, quad);
    
    return quad;
}



BASICBLOCK* new_bb() {
    
    BASICBLOCK* bb = (BASICBLOCK*) malloc(sizeof(BASICBLOCK));

    if(!bb) {
        fprintf(stderr, "basic block not correctly initialized\n"); 
        exit(0);
    }

    char name[64];
    sprintf(name, ".BB.%d.%d", funct_count, bb_count++);

    list_t* linklist = (list_t*) malloc(sizeof(list_t));
    list_init(linklist); 

    bb->name = strdup(name);
    bb->quad_list = linklist;

    return bb;
}
