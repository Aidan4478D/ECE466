#ifndef _AST_H
#define _AST_H

#define MAX_CHILDREN 4096

struct astnode {

    int type;
    struct *astnode *pointers[MAX_CHILDREN];
    int n_children;
    union astnode_values {
        struct astnode_binop binop; 
    } val;

};



#endif 

