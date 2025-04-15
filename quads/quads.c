#include "quads.h"
#include "helpers/printing.h"

int funct_count = 0;
int bb_count = 0;

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
        case BINOP_N:
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
