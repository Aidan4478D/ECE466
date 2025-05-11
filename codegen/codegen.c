#include "codegen.h"
#include "helpers/printing.h"

#include <string.h>

// Questions:
// can I use this like a(%rip) relative mode guy everywhere?

// where BB is the starting bb in the flow
void generate_asm(BASICBLOCK* bb) {

    /*printf("\t.file \"%s\"\n", file_name);*/

    // traverse through all basic blocks
    while(bb) {
        printf("%s:\n", bb->name);
        printf("\tpushq %%rbp\t\t# associate rbp with symbol %s\n", bb->name); 
        printf("\tmovq %%rsp, %%rbp\t\t# set up stack frame pointer\n\n");

        fprintf(stderr, "printing BB %s\n", bb->name);

        // traverse through all quads within basicblock
        while(!list_is_empty(bb->quad_list)) {
            QUAD* quad = (QUAD*) list_remove_head(bb->quad_list);
            printf("\t# QUAD ANALYZED: ");
            print_quad(quad);
            quad_to_asm(quad);
            printf("\n");
        }
        list_destroy(bb->quad_list);
        bb = bb->next;

        printf("\tpopq %%rbp\n");
    }
}

char* get_qnode_output(QNODE* qnode) {

    char buf[1024];

    fprintf(stderr, "type is %s\n", get_qnode_type(qnode->type)); 

    switch(qnode->type) {
        case TEMP_Q:
            // r10 and r11 registers
            sprintf(buf, "%%r10");
            return strdup(buf);

        case VAR_Q:
            if (qnode->ast_node && qnode->ast_node->type == IDENT_N) {
                // this might be sketchy as variables aren't really immediates but it gets the job done
                if (qnode->sym && qnode->sym->type == FUNCT_SYM) {
                    sprintf(buf, "$%s", qnode->ast_node->ident.name);
                } 
                else {
                    /*if(qnode->sym && qnode->sym->is_param) sprintf(buf, "%s", qnode->ast_node->ident.name, "param");*/
                    /*else sprintf(buf, "%s{%s}", qnode->ast_node->ident.name, (qnode->scope == FILE_SCOPE ? "global" : "lvar"));*/
                    fprintf(stderr, "var: %s, scope: %s, offset: %d\n", qnode->ast_node->ident.name, qnode->sym->is_param ? "param" : qnode->scope == FILE_SCOPE ? "global" : "lvar", qnode->sym->stack_offset);
                    if(((qnode->sym && qnode->sym->is_param) || qnode->scope) != FILE_SCOPE) return get_memory_operand(qnode);
                    else if(qnode->scope == FILE_SCOPE) sprintf(buf, "%s{%s}", qnode->ast_node->ident.name, (qnode->scope == FILE_SCOPE ? "global" : "lvar"));
                }
            } 
            return strdup(buf);

        case IMM_Q:
            sprintf(buf, "$%lld", qnode->ast_node->number.num_meta._int);
            return strdup(buf);

        case BB_Q:
            sprintf(buf, "%s", qnode->bb->name);
            return strdup(buf);

        case DESC_Q:
            sprintf(buf, "%s", qnode->descriptor);
            return strdup(buf);

        default:
            fprintf(stderr, "node type %d does not exist\n", qnode->type);
            return NULL;
    }
}

char *get_memory_operand(QNODE *q) {
    char buf[128];

    // Only VAR_Q makes sense here
    if (q->type != VAR_Q) {
        fprintf(stderr, "get_memory_operand on non-VAR_Q\n");
        return strdup("ERROR");
    }

    SYMBOL *sym = q->sym;
    // globals
    if (sym->scope->scope == FILE_SCOPE) sprintf(buf, "%s(%%rip)", q->ast_node->ident.name);

    // local vars & params
    else sprintf(buf, "%d(%%rbp)", sym->stack_offset);
    
    return strdup(buf);
}


void quad_to_asm(QUAD* quad) {
    QNODE* dest = quad->destination;
    QNODE* src1 = quad->src1;
    QNODE* src2 = quad->src2; 

    switch(quad->oc) {
        case LOAD_OC:
            fprintf(stderr, "LOAD_OC detected!\n");
            break;
        case STORE_OC:
            fprintf(stderr, "STORE_OC detected!\n");
            break;
        case LEA_OC:
            fprintf(stderr, "LEA_OC detected!\n");
            break;
        case MOV_OC:
            // just do this in case two symbols
            char* out = src1->type == VAR_Q ? get_memory_operand(src1) : get_qnode_output(src1);
            printf("\tmovl %s, %%ecx\n", out);
            printf("\tmovl %%ecx, %s\n", get_qnode_output(dest));
            break;

        case BR_OC:
            fprintf(stderr, "BR_OC detected!\n");
            break;
        case CMP_OC:
            fprintf(stderr, "CMP_OC detected!\n");
            break;
        case BRGT_OC:
            fprintf(stderr, "BRGT_OC detected!\n");
            break;
        case BRLT_OC:
            fprintf(stderr, "BRLT_OC detected!\n");
            break;
        case BRGE_OC:
            fprintf(stderr, "BRGE_OC detected!\n");
            break;
        case BRLE_OC:
            fprintf(stderr, "BRLE_OC detected!\n");
            break;
        case BREQ_OC:
            fprintf(stderr, "BREQ_OC detected!\n");
            break;
        case BRNE_OC:
            fprintf(stderr, "BRNE_OC detected!\n");
            break;


        case ADD_OC:
            fprintf(stderr, "ADD_OC detected!\n");
            printf("\tmovl %s, %%edx\n", get_qnode_output(src1));
            printf("\tmovl %s, %%eax\n", get_qnode_output(src2));
            printf("\taddl %%edx, %%eax\n");
            printf("\tmovl %%eax, %s\n", get_qnode_output(dest));

            break;
        case SUB_OC:
            fprintf(stderr, "SUB_OC detected!\n");
            printf("\tmovl %s, %%eax\n", get_qnode_output(src1));
            printf("\tsubl %s, %%eax\n", get_qnode_output(src2));
            printf("\tmovl %%eax, %s\n", get_qnode_output(dest));

            break;
        case MUL_OC:
            fprintf(stderr, "MUL_OC detected!\n");
            printf("\tmovl %s, %%eax\n", get_qnode_output(src1));
            printf("\tmovl %s, %%edx\n", get_qnode_output(src2));
            printf("\timull %%edx, %%eax\n");
            printf("\tmovl %%eax, %s\n", get_qnode_output(dest));

            break;
        case DIV_OC:
            fprintf(stderr, "DIV_OC detected!\n");
            printf("\tmovl %s, %%eax\n", get_qnode_output(src1));
            printf("\tcltd\n"); // convert signed long to double
            printf("\tidivl %s\n", get_qnode_output(src2));
            printf("\tmovl %%eax, %s\n", get_qnode_output(dest));

            break;
        case MOD_OC:
            fprintf(stderr, "MOD_OC detected!\n");

            printf("\tmovl %s, %%eax\n", get_qnode_output(src1));
            printf("\tcltd\n"); // convert signed long to double
            printf("\tidivl %s\n", get_qnode_output(src2));
            printf("\tmovl %%edx, %s\n", get_qnode_output(dest));
            printf("\tmovl %s, %%eax\n", get_qnode_output(dest));

            break;

        case CALL_OC:
            fprintf(stderr, "CALL_OC detected!\n");
            break;
        case ARG_OC:
            fprintf(stderr, "ARG_OC detected!\n");
            break;
        case RETURN_OC:
            fprintf(stderr, "RETURN_OC detected!\n");
            break;
        
        default:
            fprintf(stderr, "quad opcode %d does not exist\n", quad->oc);
            return;
    }
}
