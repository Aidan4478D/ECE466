#include "codegen.h"
#include "helpers/printing.h"
#include "helpers/linklist.h"

#include <fcntl.h>
#include <unistd.h>

#include <string.h>

// Questions:
// can I use this like a(%rip) relative mode guy everywhere?

char* temp_registers[] = {"%%ebx", "%%edi", "%%esi"};

// since I already do args in reverse, insert the new args into a list
list_t* arg_list;

// where BB is the starting bb in the flow
void generate_asm(BASICBLOCK* bb, char* fn_name) {

    // redirect stdout -> out_file
    int saved_stdout = dup(STDOUT_FILENO);

    // Redirect stdout to out_file for assembly output
    if (dup2(fileno(out_file), STDOUT_FILENO) < 0) {
        perror("Failed to redirect stdout to out_file");
        exit(1);
    }
    
    arg_list = (list_t*) malloc(sizeof(list_t));
    list_init(arg_list);

    printf("\t.section .rodata\n");

    while (!list_is_empty(string_literals)) {
        QNODE* current = list_remove_head(string_literals);
        printf(".LC%d:\n", current->str_label_no);
        printf("\t.string \"%s\"\n", current->descriptor);
    }
    printf("\t.text\n");

    // traverse through all basic blocks
    while(bb) {
        printf("\t.globl %s\n", fn_name);
        printf("\t.type %s, @function\n", fn_name);
        printf("%s:\n", fn_name);
        printf("%s:\n", bb->name);
        printf("\tpushl %%ebp\t\t# associate ebp with symbol %s\n", bb->name); 
        printf("\tmovl %%esp, %%rbp\t\t# set up stack frame pointer\n\n");

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

        /*printf("\tpopq %%rbp\n");*/
    }
    fflush(stdout);
    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);
}

char* get_qnode_output(QNODE* qnode) {

    char buf[1024];

    fprintf(stderr, "type is %s\n", get_qnode_type(qnode->type)); 

    switch(qnode->type) {
        case TEMP_Q:
            // rotate the 3 "long term" (now temporary registers heheh) around and they hopefully don't collide
            // just hope we don't have more than 3 temps in one function!
            int reg_index = qnode->tmp_id % 3;
            return strdup(temp_registers[reg_index]);

        case VAR_Q:
            if (qnode->ast_node && qnode->ast_node->type == IDENT_N) {
                // this might be sketchy as variables aren't really immediates but it gets the job done
                if (qnode->sym && qnode->sym->type == FUNCT_SYM) {
                    sprintf(buf, "$%s", qnode->ast_node->ident.name);
                } 
                else {
                    fprintf(stderr, "var: %s, scope: %s, offset: %d\n", qnode->ast_node->ident.name, qnode->sym->is_param ? "param" : qnode->scope == FILE_SCOPE ? "global" : "lvar", qnode->sym->stack_offset);
                    if(qnode->sym && qnode->scope != FILE_SCOPE) sprintf(buf, "%d(%%ebp)", qnode->sym->stack_offset);
                    else sprintf(buf,"%s", qnode->ast_node->ident.name);
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

        case STR_Q:
            sprintf(buf, "$.LC%d", qnode->str_label_no); 
            return strdup(buf);


        default:
            fprintf(stderr, "node type %d does not exist\n", qnode->type);
            return NULL;
    }
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
            printf("\tmovl %s, %%ecx\n", get_qnode_output(src1));
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
            while(!list_is_empty(arg_list))
                printf("\tpushl %s\n", get_qnode_output(list_remove_head(arg_list)));
            
            printf("\tcall %s\n", get_qnode_output(src1));
            printf("\taddl $%d, %%esp\n", atoi(get_qnode_output(src2)) * 4);
            break;
        case ARG_OC:
            fprintf(stderr, "ARG_OC detected!\n");
            // src 1: position, src2: value
            list_insert_tail(arg_list, src2);
            printf("\t# pushed arg #%s into arg list\n", get_qnode_output(src1));
            break;
        case RETURN_OC:
            fprintf(stderr, "RETURN_OC detected!\n");
    
            // put ret value into eax register
            if(src1) printf("\tmovl %s, %%eax\n", get_qnode_output(src1));
            printf("\tleave\n");
            printf("\tret\n");

            break;
        
        default:
            fprintf(stderr, "quad opcode %d does not exist\n", quad->oc);
            return;
    }
}
