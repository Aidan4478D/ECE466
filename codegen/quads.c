#include <string.h>
#include <stdlib.h>

#include "quads.h"
#include "symtable.h"
#include "codegen.h"

#include "helpers/printing.h"
#include "helpers/stack.h"

#include "y.tab.h"

int funct_count = 0;
int bb_count = 0;
int tmp_count = 0;

BASICBLOCK* cur_bb;
BASICBLOCK* last_bb = NULL;

stack_t* loop_stack;

// QUESTIONS:
// - is size of a pointer 8 even nested
// - do we have to write out whole sizeof operation (with mults and adds)
// - can I just assume pointers are in the left operand

BASICBLOCK* create_quads(ast_node_t* listnode) {
    
    // stack of loop infos
    loop_stack = (stack_t*) malloc(sizeof(stack_t));
    stack_init(loop_stack);

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

    printf("\n---------------------------------------------\n"); 
    printf("ASM generation for BB %s\n", bb->name); 
    printf("---------------------------------------------\n"); 
    fprintf(stderr, "=========== ASM GENERATION ============\n");
    generate_asm(bb); // generate assembly here

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
            printf("%%T%d", qnode->tmp_id);
            break;
        case IMM_Q: {
            if(qnode->ast_node->type == NUMBER_N) {
                ast_node_t* node = qnode->ast_node;
                // i know i'm not printing sign info but just keeping these just in case i do
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
                // this might be sketchy as variables aren't really immediates but it gets the job done
                if (qnode->sym && qnode->sym->type == FUNCT_SYM) {
                    printf("$%s", qnode->ast_node->ident.name);
                } 
                else {
                    if(qnode->sym && qnode->sym->is_param) printf("%s{%s}", qnode->ast_node->ident.name, "param");
                    else printf("%s{%s}", qnode->ast_node->ident.name, (qnode->scope == FILE_SCOPE ? "global" : "lvar"));
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
        case BINOP_N: 
            // handled within r_value
            // would be kinda weird to have a binary operator on its own
            fprintf(stderr, "BINARY OP detected!\n");
            break;
        case UNOP_N:
            // postinc/postdec stuff
            fprintf(stderr, "UNARY OP detected!\n"); 
            create_rvalue(node, NULL);
            break;
        case ASSIGNOP_N:
            fprintf(stderr, "ASSIGN OP detected!\n"); 
            create_assignment(node);
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
            create_while(node);
            break;
        case DOWHILE_N:
            fprintf(stderr, "DO WHILE detected!\n");
            create_dowhile(node);
            break;
        case FOR_N:
            fprintf(stderr, "FOR detected!\n");
            create_for(node);
            break;
        case BREAK_N: {
            fprintf(stderr, "BREAK detected!\n"); 
            if (stack_is_empty(loop_stack)) {
                fprintf(stderr, "Break outside loop\n");
                exit(1);
            }
            loop_info_t* info = (loop_info_t*) stack_peek(loop_stack);
            emit(BR_OC, NULL, new_bb_qnode(info->break_target), NULL);
            break;
        }
        case CONTINUE_N: {
            fprintf(stderr, "CONTINUE detected!\n"); 
            if (stack_is_empty(loop_stack)) {
                fprintf(stderr, "Continue outside loop\n");
                exit(1);
            }
            loop_info_t* info = (loop_info_t*) stack_peek(loop_stack);
            emit(BR_OC, NULL, new_bb_qnode(info->continue_target), NULL);
            break;
        }
        case RETURN_N:
            fprintf(stderr, "RETURN detected!\n");

            if (node->return_node.expression) {
                QNODE* ret_val = create_rvalue(node->return_node.expression, NULL);
                emit(RETURN_OC, ret_val, NULL, NULL);
            } 
            else emit(RETURN_OC, NULL, NULL, NULL);
            
            break;
        case DECL_N:
            // don't generate quads for declarations (would be considered an assignop ig?)
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
            QNODE* left = create_rvalue(node->genop.left, NULL);
            QNODE* right = create_rvalue(node->genop.right, NULL);
            int is_pointer_arith = 0;
    
            // this is kinda sketch and I could probably make it easier by making a function but I don't want to as of right now
            // so don't have to repeat the logic in case pointer operator is on the right
            if(left->sym && (left->sym->node->type == POINTER_N || left->sym->node->type == ARRAY_N)) {
                /*fprintf(stderr, "pointer arith detected (left)!\n");*/
                is_pointer_arith = 1;
            }
            else if(right->sym && (right->sym->node->type == POINTER_N || right->sym->node->type == ARRAY_N)) {
                /*fprintf(stderr, "pointer arith detected (right)!\n");*/
                QNODE* tmp = right;
                right = left;
                left = tmp;
                is_pointer_arith = 1;
            }
            else is_pointer_arith = 0;
                
            
            // assumes pointer is the left operand
            if (node->genop.op == '+' && is_pointer_arith) {
                ast_node_t* pointed_to_type = get_pointed_to_type(left->sym->node);

                if (!pointed_to_type) {
                    fprintf(stderr, "invalid pointer type for %s\n", left->sym->key);
                    exit(1);
                }

                int element_size = get_type_size(pointed_to_type);

                QNODE* size_qnode = new_immediate(element_size);
                QNODE* offset_qnode = new_temporary();
                QNODE* result_qnode = new_temporary();
                QNODE* base_qnode = new_temporary();
                
                emit(MUL_OC, right, size_qnode, offset_qnode);
                emit(LEA_OC, left, NULL, base_qnode);
                emit(ADD_OC, base_qnode, offset_qnode, result_qnode);

                return result_qnode;
            } 
            else {
                QNODE* result_qnode = new_temporary();
                emit(get_binop_opcode(node), left, right, result_qnode);

                return result_qnode;
            }
        }
        case UNOP_N:
            // pointer deref
            if(node->unop.op == '*') {
                QNODE* qnode = create_rvalue(node->unop.node, NULL);
                if(!target) target = new_temporary();
                emit(LOAD_OC, qnode, NULL, target);
                return target;
            }
            if(node->unop.op == '&') {
                
                // &* expr simplifies to expr
                if (node->unop.node->type == UNOP_N && node->unop.node->unop.op == '*') {
                    return create_rvalue(node->unop.node->unop.node, target);
                }
                else {
                    fprintf(stderr, "gonna print LEA\n");
                    QNODE* addr = create_rvalue(node->unop.node, NULL);
                    
                    if (!target) target = new_temporary();
                    emit(LEA_OC, addr, NULL, target);
                    return target;
                }
            }
            if(node->unop.op == PLUSPLUS || node->unop.op == MINMIN) {
                QNODE* qnode = create_rvalue(node->unop.node, NULL);
                // target IS the unop's sym
                QNODE* new_val = new_temporary();

                emit(node->unop.op == PLUSPLUS ? ADD_OC : SUB_OC, new_immediate(1), qnode, new_val);
                emit(MOV_OC, new_val, NULL, qnode);
                return target;
            }
            if(node->unop.op == SIZEOF) {
                // do you want to see the whole calculation? like ADD and MUL quads?
                int element_size = get_element_size(node->unop.node);
                QNODE* size_qnode = new_immediate(element_size);
                
                return size_qnode;
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
                //exit(1);
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
    if(node->type == IDENT_N) {
        SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
        SYMBOL* sym = st_lookup(st, node->ident.name, GENERAL_NS);
        if (!sym) {
            fprintf(stderr, "Undefined variable %s in get_address\n", node->ident.name);
            exit(-1);
        }

        ast_node_t* type_node = sym->node;
        if(type_node->type == POINTER_N || type_node->type == ARRAY_N) get_pointed_to_type(type_node);


        if (!type_node) {
            fprintf(stderr, "type for variable %s does not exist!\n", node->ident.name);
            exit(1);
        }
        
        return get_type_size(type_node);
    }
    if(node->type == POINTER_N) return 8; //that was easy
    else {
        return 0;
        fprintf(stderr, "Element size node is not an ident node! It is: %s\n", get_node_type(node->type));
    }
}



QNODE* create_lvalue(ast_node_t* node, int* mode) {

    switch(node->type) {
        case IDENT_N:
            *mode = DIRECT_MODE;
            
            SYMTABLE* st = (SYMTABLE*) stack_peek(scope_stack);
            SYMBOL* sym = st_lookup(st, node->ident.name, GENERAL_NS);

            if (!sym) {
                fprintf(stderr, "undefined variable %s\n", node->ident.name);
                exit(-1);
            }

            return new_variable(node, sym);

        case NUMBER_N: return NULL;
        case UNOP_N:
            // pointer dereference
            if(node->unop.op == '*') {
                fprintf(stderr, "pointer deref detected in L-val!\n");
                QNODE* base = create_rvalue(node->unop.node, NULL);
                if(node->unop.node->type == UNOP_N && node->unop.node->unop.op == '*') {
                    QNODE* addr = new_temporary();
                    emit(LOAD_OC, base, NULL, addr);
                    *mode = INDIRECT_MODE;
                    return addr;
                }
                *mode = INDIRECT_MODE;
                return base;
            }
            break;        
        default:
            fprintf(stderr, "invalid r-value type: %s\n", get_node_type(node->type));
            return NULL;
    }
    return NULL;
}


void create_assignment(ast_node_t* node) {

    if(node->type == ASSIGNOP_N) {
        int destmode = -1;
        QNODE* dst = create_lvalue(node->genop.left, &destmode);
        if(!dst) {
            fprintf(stderr, "error creating l-val for assignment\n");
            return;
        }
        
        QNODE* src = create_rvalue(node->genop.right, NULL);
        if(destmode == DIRECT_MODE) {
            emit(MOV_OC, src, NULL, dst);
        }
        else if (destmode == INDIRECT_MODE) {
            emit(STORE_OC, src, dst, NULL); 
        }
        else fprintf(stderr, "unsporrted LHS type in assignment: %s\na", get_node_type(node->genop.left->type));
    }
    /*fprintf(stderr, "Create assignemnt called with node type: %s\n", get_node_type(node->type));*/
}


void create_condexpr(ast_node_t* expr, BASICBLOCK* Bt, BASICBLOCK* Bf) {
    
    if (expr->type == COMPOP_N) {
        QNODE* left = create_rvalue(expr->genop.left, NULL);
        QNODE* right = create_rvalue(expr->genop.right, NULL);

        emit(CMP_OC, left, right, NULL);
        fprintf(stderr, "printed cond expr!\n");

        OPCODE branch_oc = get_branch_opcode(expr->genop.op);
        emit(branch_oc, new_bb_qnode(Bt), new_bb_qnode(Bf), NULL);
        fprintf(stderr, "finished quads for cond expr!\n");
    } 
    else fprintf(stderr, "Unsupported condition type: %s\n", get_node_type(expr->type));
}


void create_if(ast_node_t* node) {

    BASICBLOCK* Bf = new_bb();
    BASICBLOCK* Bn = node->if_node.else_statement ? new_bb() : Bf;

    if (node->if_node.then_statement->type == CONTINUE_N) {
        if (stack_is_empty(loop_stack)) {
            fprintf(stderr, "Continue outside loop\n");
            exit(1);
        }
        loop_info_t* info = (loop_info_t*) stack_peek(loop_stack);
        create_condexpr(node->if_node.condition, info->continue_target, Bf);
    } 
    else if (node->if_node.then_statement->type == BREAK_N) {
        if (stack_is_empty(loop_stack)) {
            fprintf(stderr, "Break outside loop\n");
            exit(1);
        }
        loop_info_t* info = (loop_info_t*) stack_peek(loop_stack);
        create_condexpr(node->if_node.condition, info->break_target, Bf);
    } 
    else {
        BASICBLOCK* Bt = new_bb();
        create_condexpr(node->if_node.condition, Bt, Bf);
        cur_bb = Bt;
        create_statement(node->if_node.then_statement);
        link_bb(cur_bb, ALWAYS, Bn, NULL);
    }

    if (node->if_node.else_statement) {
        cur_bb = Bf;
        create_statement(node->if_node.else_statement);
        link_bb(cur_bb, ALWAYS, Bn, NULL);
        cur_bb = Bn;
    } 
    else cur_bb = Bf;
}

void create_for(ast_node_t* node) {

    BASICBLOCK* B_cont = new_bb();
    BASICBLOCK* B_body = new_bb();
    BASICBLOCK* B_inc = new_bb();
    BASICBLOCK* B_break = new_bb();

    // set up info struct and push onto stack
    loop_info_t* info = (loop_info_t*) malloc(sizeof(loop_info_t));

    // after continuing in for, increment->eval cond->go from there
    info->continue_target = B_inc;
    info->break_target = B_break;
    stack_push(loop_stack, info);

    /*fprintf(stderr, "successfully created loop info\n");*/

    // initialization
    create_assignment(node->for_node.init);
    link_bb(cur_bb, ALWAYS, B_cont, NULL);

    // condition
    cur_bb = B_cont;
    create_condexpr(node->for_node.condition, B_body, B_break);

    // body
    cur_bb = B_body;
    create_statement(node->for_node.body);
    link_bb(cur_bb, ALWAYS, B_inc, NULL);

    // increment
    cur_bb = B_inc;
    create_statement(node->for_node.increment);
    link_bb(cur_bb, ALWAYS, B_cont, NULL);
    
    stack_pop(loop_stack);
    cur_bb = B_break;
}


void create_while(ast_node_t* node) {

    BASICBLOCK* B_cont = new_bb();
    BASICBLOCK* B_body = new_bb();
    BASICBLOCK* B_break = new_bb();

    loop_info_t* info = (loop_info_t*) malloc(sizeof(loop_info_t));

    // after continuing in while, evaluate condition again
    info->continue_target = B_cont; 
    info->break_target = B_break;
    stack_push(loop_stack, info);
    
    // initialization
    link_bb(cur_bb, ALWAYS, B_cont, NULL);
    /*fprintf(stderr, "linked cur->cond\n");*/

    cur_bb = B_cont;
    create_condexpr(node->while_node.condition, B_body, B_break);
    /*fprintf(stderr, "finished cond expr\n");*/

    // condition
    cur_bb = B_body;
    create_statement(node->while_node.body);
    /*fprintf(stderr, "created statement for body\n");*/
    link_bb(cur_bb, ALWAYS, B_cont, NULL);
    /*fprintf(stderr, "linked cur -> inc\n");*/

    // body
    stack_pop(loop_stack);
    cur_bb = B_break;
}


void create_dowhile(ast_node_t* node) {

    BASICBLOCK* B_body = new_bb();
    BASICBLOCK* B_cont = new_bb();
    BASICBLOCK* B_break = new_bb();

    loop_info_t* info = (loop_info_t*) malloc(sizeof(loop_info_t));

    // after continuing in while, evaluate condition again
    info->continue_target = B_cont; 
    info->break_target = B_break;
    stack_push(loop_stack, info);
    
    // connect current to body
    link_bb(cur_bb, ALWAYS, B_body, NULL);

    // body -> continue
    cur_bb = B_body;
    create_statement(node->while_node.body);
    link_bb(cur_bb, ALWAYS, B_cont, NULL);

    // continue -> body & break
    cur_bb = B_cont;
    create_condexpr(node->while_node.condition, B_body, B_break);
    
    stack_pop(loop_stack);
    cur_bb = B_break;
}

void link_bb(BASICBLOCK* cur_bb, int mode, BASICBLOCK* Bt, BASICBLOCK* Bf) {

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

    // if previous BB link itG
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

    if (!cur_bb || !cur_bb->quad_list) {
        fprintf(stderr, "Invalid cur_bb or quad_list\n");
        exit(1);
    }

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
