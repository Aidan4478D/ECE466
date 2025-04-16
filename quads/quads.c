#include "quads.h"
#include "helpers/printing.h"

int funct_count = 0;
int bb_count = 0;
int tmp_count = 0;
BASICBLOCK* cur_bb;

BASICBLOCK* create_quads(ast_node_t* list) {

    // create linked list for quads
    BASICBLOCK* bb = (BASICBLOCK*) malloc(sizeof(BASICBLOCK));
    init_bb(bb); 
        
    return bb;
}


QNODE* create_statement(ast_node_t* node) {

    QNODE* quad_node = (QNODE*) malloc(sizeof(QNODE));

    // assign what the quad node should be here
    switch(node->type) {
        case BINOP_N: //binop within genop
            break;
        case UNOP_N:
            break;
        case ASSIGNOP_N:
            break;
        case FUNCT_N:
            break;
        case FUNCTCALL_N:
            break;
        case IF_N:
            break;
        case WHILE_N:
            break;
        case DOWHILE_N:
            break;
        case FOR_N:
            break;
        case BREAK_N:
            break;
        case CONTINUE_N:
            break;
        case RETURN_N:
            break;
        default:
            fprintf(stderr, "invalid statement type: node_type = %s\n", get_node_type(node->type));
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
        case NUMBER_N:
            break;
        case STRING_N:
            break;
        case BINOP_N:
            left = create_rvalue(node->genop.left, NULL);
            right = create_rvalue(node->genop.right, NULL);
            if(!target) target = new_temporary();
            emit(get_opcode(node), left, right, target); 
            return target;
        default:
            fprintf(stderr, "invalid r-value type: %s\n", get_node_type(node->type));
            break;
    }
}

// create new node of type TEMPORARY
QNODE* new_temporary() {
 
    QNODE* node = (QNODE*) malloc(sizeof(QNODE));
    
    node->type = TEMP_Q;
    node->tmp_id = tmp_count++;
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



void init_bb(BASICBLOCK* bb) {

    if(!bb) {
        fprintf(stderr, "basic block not correctly initialized\n"); 
        exit(0);
    }

    char name[64];
    sprintf(name, ".BB.%d.%d", funct_count, bb_count++);

    list_t* linklist = (list_t*) malloc(sizeof(list_t));
    list_init(linklist); 

    bb->name = name;
    bb->quad_list = linklist;
}
