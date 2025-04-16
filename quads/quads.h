#ifndef _QUADS_H
#define _QUADS_H

#include "ast.h"
#include "symtable.h"
#include "helpers/linklist.h"

typedef enum opcode {
    LOAD_OC,
    STORE_OC,
    LEA_OC,
    MOV_OC,

    BR_OC,
    CMP_OC,
    BRGT_OC,
    BRLT_OC,
    BRGE_OC,
    BRLE_OC,
    BREQ_OC,
    BRNE_OC,

    ADD_OC,
    SUB_OC,
    MUL_OC,
    DIV_OC,
    MOD_OC,

    AND_OC,
    OR_OC,
    XOR_OC,
    NOT_OC,
    ANDAND_OC,
    OROR_OC,
    LOGNOT_OC,

    SL_OC,
    SR_OC,

    FNCALL_OC,
    RETURN_OC
} OPCODE;

// not sure the other types but I'm sure this will expand
typedef enum quad_types {
    TEMP_Q,
    VAR_Q
} QTYPE;

typedef struct quad_node {
    QTYPE type;
    int tmp_id; // for tmp nodes
    SYMBOL* sym;
    ast_node_t* ast_node;
} QNODE; 


typedef struct quad {
    OPCODE oc;
    QNODE *destination, *src1, *src2;
} QUAD;


typedef struct basic_block {
    char* name;
    list_t* quad_list; //list of quads 
    //struct basic_block *next;
} BASICBLOCK;

// input = compound statement list
BASICBLOCK* create_quads(ast_node_t* list);

void init_bb(BASICBLOCK* bb);

QNODE* create_statement(ast_node_t* node);
QNODE* create_rvalue(ast_node_t* node, QNODE* target); 
QNODE* new_temporary();

// create a bew qyad wutg guveb 4 args abd append it to list of quads
QUAD* emit(OPCODE oc, QNODE* src1, QNODE* src2, QNODE* destination); 

#endif
