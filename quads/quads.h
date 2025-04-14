#ifndef _QUADS_H
#define _QUADS_H

#include "ast.h"

enum opcode {
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
};

typedef struct quad {
    enum opcode oc;
    ast_node_t *destination, *src1, *src2;
} QUAD;

typedef struct basic_block {
    char* name;
    //list of quads
    struct basic_block *next;
} BASICBLOCK;


#endif
