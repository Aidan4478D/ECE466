#include <string.h>
#include <stdlib.h>

#include "quads.h"
#include "helpers/printing.h"
#include "symtable.h"

int funct_count = 0;
int bb_count = 0;
int tmp_count = 0;
BASICBLOCK* cur_bb;
static BASICBLOCK* last_bb = NULL;

int is_pointer_arith = 0;

BASICBLOCK* create_quads(ast_node_t* listnode) {

    // create linked list for quads
    BASICBLOCK* bb = new_bb();
    cur_bb = bb;
    last_bb = bb;

    int ret_found = 0;

    while(listnode) {
        ast_node_t* node = listnode->list.head;
        if(node->type == RETURN_N) ret_found = 1;
        if(node) create_statement(node);
        listnode = listnode->list.next;
    }
    if(!ret_found) emit(RETURN_OC, NULL, NULL, NULL);
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
    printf("\t%s ", print_opcode(quad->oc));
    if (quad->oc == LOAD_OC && quad->src1) {
        printf(" [");
        print_qnode(quad->src1);
        printf("]");
    } 
    else {
        if (quad->src1) {
            print_qnode(quad->src1);
            if (quad->src2) printf(", ");
        }
        if (quad->src2) print_qnode(quad->src2);
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
            printf("%T%d", qnode->tmp_id);
            break;
        case IMM_Q: {
            if(qnode->ast_node->type == NUMBER_N) {
                ast_node_t* node = qnode->ast_node;
                switch (node->number.num_meta.type) {
                    case INT_T:     
                        if(node->number.num_meta.sign == SIGNED_T) printf("$%lld", node->number.num_meta._int);
                        if(node->number.num_meta.sign == UNSIGNED_T) printf("$%llu", node->number.num_meta._int);
                        break;
                    case LONG_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("$%lld", node->number.num_meta._int);
                        if(node->number.num_meta.sign == UNSIGNED_T) printf("$%llu", node->number.num_meta._int);
                        break;
                    case LONGLONG_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("$%lld", node->number.num_meta._int);
                        if(node->number.num_meta.sign == UNSIGNED_T) printf("$%llu", node->number.num_meta._int);
                        break;
                    case FLOAT_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("$%f\n", node->number.num_meta._float);
                        break;
                    case DOUBLE_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("$%Lf\n", node->number.num_meta._double);
                        break;
                    case LONGDOUBLE_T:
                        if(node->number.num_meta.sign == SIGNED_T) printf("$%Lf\n", node->number.num_meta._double);
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
                if (qnode->sym && qnode->sym->type == FUNCT_SYM) {
                    printf("$%s", qnode->ast_node->ident.name); // Prefix functions with $
                } 
                else {
                    printf("%s{%s}", qnode->ast_node->ident.name, (qnode->scope == FILE_SCOPE ? "global" : "lvar"));
                }
            } 
            else printf("VAR_UNKNOWN");
            break;

        case BB_Q:
            printf("%s", qnode->bb->name);
            break;

        case DESC_Q:
            printf("%s", qnode->descriptor);
            break;

        default: printf("UNKNOWN_QNODE");
    }
}



QUAD* create_statement(ast_node_t* node) {

    QUAD* quad_node = (QUAD*) malloc(sizeof(QUAD));

    // assign what the quad node should be here
    switch(node->type) {
        
        case LIST_N: {
            fprintf(stderr, "LIST detected!\n");
            ast_node_t* current = node;
            while (current) {
                create_statement(current->list.head);
                current = current->list.next;
            }
            return NULL;
        }
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
            create_assignment(node);
            break;
        case FUNCT_N:
            fprintf(stderr, "FUNCT detected!\n"); 
            break;
        case FUNCTCALL_N:
            fprintf(stderr, "FUNCT CALL detected!\n");
            create_rvalue(node, NULL);
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

    fprintf(stderr, "node type is: %s\n", get_node_type(node->type));
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

            /*fprintf(stderr, "cur scope is %s for sym %s\n", get_scope_name(st->scope), node->ident.name); */

            if(sym) {
                qnode->sym = sym;
                qnode->scope = sym->scope->scope; //symbol's st's scope
            }
            else fprintf(stderr, "Undefined variable %s\n", node->ident.name); 
            
            qnode->type = VAR_Q;
            qnode->ast_node = node;
            return qnode;
        }
        case BINOP_N: {
            // check if it's pointer arithmetic
            if(is_pointer_arith) {
                is_pointer_arith = 0;
                return get_address(node);
            }
            else {
                QNODE* left = create_rvalue(node->genop.left, NULL);
                QNODE* right = create_rvalue(node->genop.right, NULL);
                QNODE* result_qnode = new_temporary();
                emit(get_binop_opcode(node), left, right, result_qnode);
                return result_qnode;
            }
        }
        case UNOP_N:
            // pointer deref
            if(node->unop.op == '*') {
                is_pointer_arith = 1;
                QNODE* qnode = create_rvalue(node->unop.node, NULL);
                if(!target) target = new_temporary();
                emit(LOAD_OC, qnode, NULL, target);
                return target;
            }
            if(node->unop.op == '&') {
                QNODE* qnode = create_rvalue(node->unop.node, NULL);
                if(!target) target = new_temporary();
                emit(LEA_OC, qnode, NULL, target);
                return target;
            }
            break;

        case POINTER_N: {
            fprintf(stderr, "POINTER DETECTED!\n");
            QNODE* addr = create_rvalue(node->pointer.next, NULL);
            if(!target) target = new_temporary();
            emit(LOAD_OC, addr, NULL, target);
            return target;
        }

        case FUNCTCALL_N: {
            if (!target) target = new_temporary();

            SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
            SYMBOL* sym = st_lookup(st, node->funct_call.name->ident.name, GENERAL_NS);
            if (!sym || sym->type != FUNCT_SYM) {
                fprintf(stderr, "Undefined function %s\n", node->funct_call.name->ident.name);
                exit(1);
            }
            QNODE* func_qnode = new_variable(node->funct_call.name, sym);

            // emit arg quads and count arguments
            int num_args = 0;
            if (node->funct_call.params) num_args = emit_args(node->funct_call.params, 0);

            // put num_args into string so can use it as descriptor
            char descriptor[20];
            sprintf(descriptor, "%d", num_args);

            QNODE* args_qnode = new_descriptor(descriptor);
            emit(CALL_OC, func_qnode, args_qnode, target);

            return target;
        }

        default:
            fprintf(stderr, "invalid r-value type: %s\n", get_node_type(node->type));
            return NULL;
    }
    return NULL;
}


int get_element_size(ast_node_t* node) {
    // Look up node->ident.name in symbol table to get type
    SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
    SYMBOL* sym = st_lookup(st, node->ident.name, GENERAL_NS);
    if (!sym) {
        fprintf(stderr, "Undefined variable %s in get_address\n", node->ident.name);
        exit(-1);
    }

    fprintf(stderr, "Assuming element size is 4 for array/pointer access\n");
    return 4; // Size of int
}


QNODE* get_address(ast_node_t* node) {
    QNODE *address_qnode;

    switch(node->type) {
        case IDENT_N:
            address_qnode = new_temporary();
    
            // get symbol
            SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
            SYMBOL* sym = st_lookup(st, node->ident.name, GENERAL_NS);
            if (!sym) {
                fprintf(stderr, "Undefined variable %s in get_address\n", node->ident.name);
                exit(-1);
            }

            QNODE* var_qnode = new_variable(node, sym);
            emit(LEA_OC, var_qnode, NULL, address_qnode);

            return address_qnode;

        case UNOP_N: 
            if (node->unop.op == '*') {
                address_qnode = create_rvalue(node->unop.node, NULL);
                return address_qnode;
            }
            break;

        case BINOP_N: {
            ast_node_t* base_node = node->genop.left;
            ast_node_t* index_node = node->genop.right;

            QNODE* base_qnode = get_address(base_node);
            QNODE* index_qnode = create_rvalue(index_node, NULL);

            int element_size = get_element_size(base_node);
            QNODE* size_qnode = new_immediate(element_size);

            QNODE* offset_qnode = new_temporary();
            emit(MUL_OC, index_qnode, size_qnode, offset_qnode);

            address_qnode = new_temporary();
            emit(ADD_OC, base_qnode, offset_qnode, address_qnode);

            return address_qnode;
        }
        break;

        default:
            fprintf(stderr, "Unsupported node type (%s) in get_address\n", get_node_type(node->type));
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
            fprintf(stderr, "pointer deref detected!\n");
            if(node->unop.op == '*') {
                *mode = INDIRECT_MODE;
                return create_rvalue(node->unop.node, NULL); 
            }
        default:
            fprintf(stderr, "invalid r-value type: %s\n", get_node_type(node->type));
            return NULL;
    }
}


void create_assignment(ast_node_t* node) {

    if(node->type == ASSIGNOP_N) {
        int destmode;
        QNODE* dst = create_lvalue(node->genop.left, &destmode);

        if(destmode == DIRECT_MODE) {
            QNODE* tmp = create_rvalue(node->genop.right, dst);
            if(tmp != dst) emit(MOV_OC, tmp, NULL, dst);
        }
        else if (node->genop.left->type == UNOP_N || node->genop.left->unop.op == '*') {
            QNODE* t1 = create_rvalue(node->genop.left->unop.node, NULL);
            emit(STORE_OC, t1, dst, NULL); 
        }
        else fprintf(stderr, "unsporrted LHS type in assignment: %s\na", get_node_type(node->genop.left->type));
    }
    /*fprintf(stderr, "Create assignemnt called with node type: %s\n", get_node_type(node->type));*/
}


/*typedef struct ast_node_function {*/
    /*char* name;*/
    /*ast_node_t* return_type;  //function types: return type*/
    /*ast_node_t* params; //function types: params*/
/*} ast_node_function_t;*/

/*typedef struct ast_node_function_call {*/
    /*ast_node_t* name;*/
    /*ast_node_t* params;*/
/*} ast_node_function_call_t;*/


void create_condexpr(ast_node_t* expr, BASICBLOCK* Bt, BASICBLOCK* Bf) {
    
    if (expr->type == COMPOP_N) {
        QNODE* left = create_rvalue(expr->genop.left, NULL);
        QNODE* right = create_rvalue(expr->genop.right, NULL);

        emit(CMP_OC, left, right, NULL);

        OPCODE branch_oc = get_branch_opcode(expr->genop.op);
        emit(branch_oc, new_bb_qnode(Bt), new_bb_qnode(Bf), NULL);
    } 
    else fprintf(stderr, "Unsupported condition type: %s\n", get_node_type(expr->type));
}


void create_if(ast_node_t* node) {

    BASICBLOCK* Bt = new_bb();
    BASICBLOCK* Bf = new_bb();
    BASICBLOCK* Bn = node->if_node.else_statement ? new_bb() : Bf;

    create_condexpr(node->if_node.condition, Bt, Bf);
    cur_bb = Bt;

    create_statement(node->if_node.then_statement);
    link_bb(cur_bb, ALWAYS, Bn, NULL);

    if(node->if_node.else_statement) {
        cur_bb = Bf;
        create_statement(node->if_node.else_statement);
        link_bb(cur_bb, ALWAYS, Bn, NULL);
    }
    cur_bb = Bn;
}

void link_bb(BASICBLOCK* cur_bb, MODE mode, BASICBLOCK* Bt, BASICBLOCK* Bf) {

    if (mode == ALWAYS) {
        emit(BR_OC, NULL, new_bb_qnode(Bt), NULL); // Unconditional jump to Bt (Bn in context)
    } 
    else {
        fprintf(stderr, "Unsupported link mode: %d\n", mode);
    }
}


// create new node of type TEMPORARY
BASICBLOCK* new_bb() {
    
    BASICBLOCK* bb = (BASICBLOCK*) malloc(sizeof(BASICBLOCK));

    if(!bb) {
        fprintf(stderr, "basic block not correctly initialized\n"); 
        exit(0);
    }

    char name[64];
    sprintf(name, ".BB.%d.%d", funct_count, bb_count);

    list_t* linklist = (list_t*) malloc(sizeof(list_t));
    list_init(linklist); 

    bb->name = strdup(name);
    bb->quad_list = linklist;
    bb->bb_num = bb_count++;
    bb->funct_num = funct_count;

    bb->next = NULL; // Initialize next to NULL

    // if previous BB link it
    if (last_bb) {
        last_bb->next = bb;
    }
    last_bb = bb;

    return bb;
}


QNODE* new_temporary() {
 
    QNODE* node = (QNODE*) malloc(sizeof(QNODE));
    
    node->type = TEMP_Q;
    node->tmp_id = tmp_count++;

    node->scope = -1;
    node->sym = NULL;
    node->ast_node = NULL;
    node->bb = NULL;
    
    return node;
}


QNODE* new_bb_qnode(BASICBLOCK* bb) {

    QNODE* node = (QNODE*) malloc(sizeof(QNODE));

    node->type = BB_Q;
    node->bb = bb;

    node->tmp_id = -1;
    node->sym = NULL;
    node->ast_node = NULL;
    node->scope = -1;

    return node;
}

QNODE* new_immediate(int value) {

    QNODE* qnode = (QNODE*)malloc(sizeof(QNODE));
    qnode->type = IMM_Q;

    ast_node_t* size_node = (ast_node_t*)malloc(sizeof(ast_node_t));

    size_node->type = NUMBER_N;
    size_node->number.num_meta.type = INT_T;
    size_node->number.num_meta.sign = SIGNED_T;
    size_node->number.num_meta._int = value;

    qnode->ast_node = size_node;

    return qnode;
}

QNODE* new_variable(ast_node_t* node, SYMBOL* sym) {

    QNODE* qnode = (QNODE*) malloc(sizeof(QNODE));

    qnode->type = VAR_Q;
    qnode->ast_node = node;
    qnode->sym = sym;
    qnode->scope = sym->scope->scope;

    return qnode;
}

QNODE* new_descriptor(char* name) {

    QNODE* qnode = (QNODE*) malloc(sizeof(QNODE));

    qnode->type = DESC_Q;
    qnode->descriptor = strdup(name);

    return qnode;
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


int emit_args(ast_node_t* param_list, int pos) {
    // recursive arg print
    if (param_list == NULL) return 0;
    int count = emit_args(param_list->list.next, pos + 1);

    ast_node_t* param_node = param_list->list.head;

    QNODE* value = create_rvalue(param_node, NULL);

    char descriptor[20];
    sprintf(descriptor, "%d", pos);
    QNODE* position = new_descriptor(descriptor);

    emit(ARG_OC, position, value, NULL);

    return count + 1;
}
