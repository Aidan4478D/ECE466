#ifndef _QUADS_H
#define _QUADS_H

#include "ast.h"
#include "helpers/linklist.h"
#include "symtable.h"

#define ALWAYS 3

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

    // not gonna implement
    //AND_OC,
    //OR_OC,
    //XOR_OC,
    //NOT_OC,
    //ANDAND_OC,
    //OROR_OC,
    //LOGNOT_OC,
    //SL_OC,
    //SR_OC,

    CALL_OC,
    ARG_OC,
    RETURN_OC
} OPCODE;

typedef enum modes {
    DIRECT_MODE,
    INDIRECT_MODE,
} MODE;

// not sure the other types but I'm sure this will expand
typedef enum quad_types {
    TEMP_Q, // temporaty
    VAR_Q,  // variable
    IMM_Q,  // immediate
    BB_Q,   // basic block
    DESC_Q, // descriptor (for arg numbers)
    STR_Q   // strings for string literal
} QTYPE;

struct basic_block;

typedef struct quad_node {
    QTYPE type;
    int tmp_id; // for tmp nodes

    SCOPETYPE scope;
    SYMBOL* sym;
    ast_node_t* ast_node;
    
    struct basic_block* bb; // for block nodes
    
    char* descriptor;
    int str_label_no; //for strings
} QNODE; 


typedef struct quad {
    OPCODE oc;
    QNODE *destination, *src1, *src2;
} QUAD;


typedef struct basic_block {
    char* name;
    list_t* quad_list; //list of quads 
    int bb_num;
    int funct_num;
    struct basic_block* next;

    int stack_size;
} BASICBLOCK;


typedef struct loop_info {
    BASICBLOCK* continue_target;
    BASICBLOCK* break_target;
} loop_info_t;

// input = compound statement list
BASICBLOCK* create_quads(ast_node_t* list);
BASICBLOCK* new_bb();

QNODE* create_rvalue(ast_node_t* node, QNODE* target); 
QNODE* create_lvalue(ast_node_t* node, int* mode);
QNODE* get_address(ast_node_t* node);

QNODE* new_temporary();
QNODE* new_bb_qnode();
QNODE* new_immediate();
QNODE* new_variable();
QNODE* new_descriptor();

// function calls handled in create_rvalue
// goal of gen_condexpr is to evaluate the expression and branch to either the true target or the false target
void create_condexpr(ast_node_t* expr, BASICBLOCK* Bt, BASICBLOCK* Bf); 
void create_assignment(ast_node_t* node);
void create_if(ast_node_t* node);
void create_for(ast_node_t* node);
void create_while(ast_node_t* node);
void create_dowhile(ast_node_t* node);

void link_bb(BASICBLOCK* cur_bb, int mode, BASICBLOCK* Bt, BASICBLOCK* Bf); 

// create a new quad with 4 args and append it to list of quads
QUAD* emit(OPCODE oc, QNODE* src1, QNODE* src2, QNODE* destination); 
int emit_args(ast_node_t* param_list, int pos);
QUAD* create_statement(ast_node_t* node);

void print_all(BASICBLOCK* bb);
void print_bb(BASICBLOCK* bb);
void print_quad(QUAD* quad);
void print_qnode(QNODE* node);

int get_element_size(ast_node_t* node);

// list of string literals for code gen
extern list_t* string_literals;
extern int str_label_cnt;

#endif
