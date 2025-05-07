#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "ast.h"

#include "helpers/hash.h"

#include "helpers/stack.h"
extern stack_t* scope_stack;


// not doing prototypes
typedef enum scope_types {
    FILE_SCOPE,
    FUNCT_SCOPE,
    BLOCK_SCOPE,
    PROTO_SCOPE
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
    UNION_SYM,
    MEMBER_SYM,
    LABEL_SYM
} SYMTYPE;



typedef struct sym_table {
    SCOPETYPE scope;
    ht_t* ht; 

    // for keeping track of where scope is defined
    int start_line;
    char* start_file;

    struct sym_table* outer;
    int is_struct_scope;
} SYMTABLE;


typedef struct symbol {
    char* key; // key for hash table

    // symbol specific line num & file name, parent scope st
    SYMTABLE* scope;
    char* file_name;
    int line_num;

    NAMESPACE name_space; //namespace is a keyword :(
    SYMTYPE type;
    STGCLASS stg_class;

    ast_node_t* node;
    struct symbol* next; //allow for chained symbols like int x, y 
    
    // for structs and unions
    int is_complete;  // use for "seen" field in labels as well
    SYMTABLE* mini_st;
    struct symbol* parent_sym; //purely for printing purposes so we can trace where struct/union is defined
    int is_param; // to keep track for quads
} SYMBOL;


SYMTABLE* st_create(SCOPETYPE scope, SYMTABLE* outer); 
int st_install(SYMTABLE* st, SYMBOL* sym);
SYMBOL* st_new_symbol(char* key, ast_node_t* node, NAMESPACE ns, SYMTYPE type, STGCLASS stg_class, SYMTABLE* st, char* fname, int lineno);

// lookup thorugh each ascending symbol table
SYMBOL* st_lookup(SYMTABLE* st, char* key, NAMESPACE ns);

// lookup in a specific symbol table
SYMBOL* st_lookup_single(SYMTABLE* st, char* key, NAMESPACE ns);

#endif

