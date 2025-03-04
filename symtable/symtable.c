#include <stdio.h>
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
void st_install(SYMTABLE* st, SYMBOL* sym) {
    
    // implement namespace stuff later
    switch(ht_insert(st->ht, sym->key, sym)) {
        case 0: fprintf(stderr, "Inserted key %s into hash table successfully\n", sym->key); break;
        case 1: fprintf(stderr, "Error inserting into hash table: key %s already exists.\n", sym->key); break;
        case 2: fprintf(stderr, "Error inserting into hash table: rehashing failed\n"); break;
        default: fprintf(stderr, "Unknown return from hash table insert\n"); break;
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
