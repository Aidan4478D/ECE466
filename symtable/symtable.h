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
    TAG,
    LABEL,
    MEMBER,
    GENERAL // everything else
} NAMESPACE;

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


// this also includes typdefs but that's not required
typedef enum storage_class {
    REG_SC,
    AUTO_SC,
    EXTERN_SC,
    STATIC_SC
} STGCLASS;

typedef struct symbol {
    char* key; // key for hash table
    int lineno;

    NAMESPACE name_space; //namespace is a keyword :(
    SYMTYPE symbol_type;

    ast_node_t* node;
}


typedef struct sym_table {
    SCOPETYPE scope;
    STGCLASS stgclass; 
    ht_t* ht; 
} SYMTABLE;



#endif
