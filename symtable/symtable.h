#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "ast.h"
#include "hash.h"

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
    GENERAL_NS // everything else
} NAMESPACE;


// this also includes typdefs but that's not required
typedef enum storage_class {
    REG_SC,
    AUTO_SC,
    EXTERN_SC,
    STATIC_SC,
    UNKNOWN_SC
} STGCLASS;


typedef enum sym_types {
    VAR_SYM,
    FUNCT_SYM,
    STRUCT_SYM,
    STAG_SYM,
    UNION_SYM,
    UTAG_SYM,
    MEMBER_SYM,
    LABEL_SYM
} SYMTYPE;


typedef struct symbol {
    char* key; // key for hash table
    int lineno;

    NAMESPACE name_space; //namespace is a keyword :(
    SYMTYPE type;
    STGCLASS stg_class;

    ast_node_t* node;
} SYMBOL;


typedef struct sym_table {
    SCOPETYPE scope;
    ht_t* ht; 
} SYMTABLE;

SYMTABLE* st_create(SCOPETYPE scope); 
int st_install(SYMTABLE* st, SYMBOL* sym);

SYMBOL* st_new_symbol(STRTYPE name, ast_node_t* node, NAMESPACE ns, SYMTYPE type, STGCLASS stg_class);

#endif
