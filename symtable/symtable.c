#include <stdio.h>
#include <stdlib.h>

#include "symtable.h"
#include "hash.h"

// create a symbol table for a given scope which can be pushed onto the sym table stack
SYMTABLE* st_create(SCOPETYPE scope) {
    SYMTABLE* st = (SYMTABLE*) malloc(sizeof(SYMTABLE));
    ht_t* ht = ht_create(1024);

    st->scope = scope;
    st->ht = ht;

    return st;
}


void st_destroy(SYMTABLE* st) {
    ht_destroy(st->ht);
    free(st); 
}


// install symbol in the scope's symbol table
int st_install(SYMTABLE* st, SYMBOL* sym) {
    
    // implement namespace stuff later
    switch(ht_insert(st->ht, sym->key, sym)) {
        case 0: fprintf(stderr, "Inserted key %s into hash table successfully\n", sym->key); return 0;
        case 1: fprintf(stderr, "Error inserting into hash table: key %s already exists.\n", sym->key); return -1;
        case 2: fprintf(stderr, "Error inserting into hash table: rehashing failed\n"); return -1;
        default: fprintf(stderr, "Unknown return from hash table insert\n"); return -1;
    }
}

SYMBOL* st_lookup(SYMTABLE* st, SCOPETYPE scope, char* key, NAMESPACE ns) {

    // return associated symbol table entry if it exists
    // otherwise return the fact that symbol doesn't exist
    //
    // operation must go through the stack of scopes until the outermost (file scope)
    // before concluding symbol doesn't exist
    
    return;
}

SYMBOL* st_new_symbol(STRTYPE key, ast_node_t* node, NAMESPACE ns, SYMTYPE type, STGCLASS stg_class) {

    SYMBOL* sym = (SYMBOL*) malloc(sizeof(SYMBOL));

    if(key.type == STRING_T) sym->key = key.string_literal;
    else sym->key = (char*) key.char_literal;

    sym->name_space = ns;
    sym->type = type;
    sym->stg_class = stg_class;

    sym->node = node;

    return sym;
}
