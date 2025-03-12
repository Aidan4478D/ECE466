#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "symtable.h"
#include "hash.h"

// create a symbol table for a given scope which can be pushed onto the sym table stack
SYMTABLE* st_create(SCOPETYPE scope, SYMTABLE* outer) {
    SYMTABLE* st = (SYMTABLE*) malloc(sizeof(SYMTABLE));
    ht_t* ht = ht_create(1024);

    st->scope = scope;
    st->ht = ht;
    st->outer = outer;

    return st;
}


void st_destroy(SYMTABLE* st) {
    ht_destroy(st->ht);
    free(st); 
}


// install symbol in the scope's symbol table
int st_install(SYMTABLE* st, SYMBOL* sym) {
    
    // implement namespace stuff later
    /*fprintf(stderr, "ket is: %s\n", sym->key); */

    switch(ht_insert(st->ht, sym->key, sym)) {
        case 0: fprintf(stderr, "Inserted key %s into hash table successfully\n", sym->key); return 0;
        case 1: fprintf(stderr, "Error inserting into hash table: key %s already exists.\n", sym->key); return -1;
        case 2: fprintf(stderr, "Error inserting into hash table: rehashing failed\n"); return -1;
        default: fprintf(stderr, "Unknown return from hash table insert\n"); return -1;
    }
}

/*void* ht_get_pointer(ht_t *ht, const char *key, bool *b);*/
SYMBOL* st_lookup(SYMTABLE* st, SCOPETYPE scope, char* key, NAMESPACE ns) {
    SYMBOL* sym = NULL;
    bool found = false;

    for (SYMTABLE* current = st; current != NULL; current = current->outer) {

        sym = (SYMBOL*) ht_get_pointer(current->ht, key, &found);
        if (found && sym != NULL) {
            if (sym->name_space == ns) {
                return sym;
            }
        }
        if (current->scope == FILE_SCOPE) break;
    }
    return NULL;
}


SYMBOL* st_new_symbol(char* key, ast_node_t* node, NAMESPACE ns, SYMTYPE type, STGCLASS stg_class) {

    SYMBOL* sym = (SYMBOL*) malloc(sizeof(SYMBOL));

    /*fprintf(stderr, "str type is: %d with key %s\n", key.type, key.string_literal); */

    sym->key = key;
    sym->name_space = ns;
    sym->type = type;
    sym->stg_class = stg_class;

    
    /*printf("key = %s, namespace = %d, type = %d, storage = %d\n", sym->key,sym->name_space, sym->type, sym->stg_class);*/

    sym->node = node;

    return sym;
}
