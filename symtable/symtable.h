#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "hash.h"
#include "ast.h"

// not doing prototypes
typedef enum scope_types {
    FILE_SCOPE,
    FUNCT_SCOPE,
    BLOCK_SCOPE
} SCOPETYPE;


typedef enum name_spaces {
    TAG_NS,
    LABEL_NS,
    MEMBER_NS,
    GENERAL_NS // function, parameter, variable
} NAMESPACE;


typedef enum sym_types {
    VAR_SYM=0,
    FUNCT_SYM,
    STRUCT_SYM,
    STAG_SYM,
    UNION_SYM,
    UTAG_SYM,
    MEMBER_SYM,
    LABEL_SYM
} SYMTYPE;



typedef struct sym_table {
    SCOPETYPE scope;
    ht_t* ht; 
    struct sym_table* outer;
} SYMTABLE;


typedef struct symbol {
    char* key; // key for hash table
    int lineno;

    NAMESPACE name_space; //namespace is a keyword :(
    SYMTYPE type;
    STGCLASS stg_class;

    ast_node_t* node;
    struct symbol* next; //allow for chained symbols
    
    int is_complete;
    SYMTABLE* mini_st;
} SYMBOL;


SYMTABLE* st_create(SCOPETYPE scope, SYMTABLE* outer); 
int st_install(SYMTABLE* st, SYMBOL* sym);
SYMBOL* st_lookup(SYMTABLE* st, SCOPETYPE scope, char* key, NAMESPACE ns);
SYMBOL* st_new_symbol(char* key, ast_node_t* node, NAMESPACE ns, SYMTYPE type, STGCLASS stg_class, SYMTABLE* st);


#endif
