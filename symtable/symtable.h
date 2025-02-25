#ifndef _SYMTABLE_H
#define _SYMTABLE_H

#include "ast.h"

typedef enum scope_types {
    FILE_SCOPE,
    FUNCT_SCOPE,
    BLOCK_SCOPE,
    PROTO_SCOPE
} SCOPETYPE;


// this also includes typdefs but that's not required
typedef enum storage_class {
    REG_SC,
    AUTO_SC,
    EXTERN_SC,
    STATIC_SC
} STGCLASS;


struct sym_table_entry {
    NODETYPE type;
    SCOPETYPE scope;
    STGCLASS stgclass; 
    int lineno;

}


#endif
