#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "symtable.h"
#include "hash.h"
#include "helpers.h"

// create a symbol table for a given scope which can be pushed onto the sym table stack
SYMTABLE* st_create(SCOPETYPE scope, SYMTABLE* outer) {
    SYMTABLE* st = (SYMTABLE*) malloc(sizeof(SYMTABLE));
    ht_t* ht = ht_create(1024);

    st->scope = scope;
    st->ht = ht;
    st->outer = outer;
    st->start_line = line_num;
    st->start_file = file_name;

    return st;
}


void st_destroy(SYMTABLE* st) {
    ht_destroy(st->ht);
    free(st); 
}


// install symbol in the scope's symbol table
int st_install(SYMTABLE* st, SYMBOL* sym) {
    
    /*fprintf(stderr, "ket is: %s\n", sym->key); */
    if (st == NULL || sym == NULL) {
        fprintf(stderr, "Invalid symbol table or symbol\n");
        return -1;
    }
    sym->scope = st;  // associate symbol with its scope

    
    bool found = false;
    SYMBOL* existing_sym = (SYMBOL*) ht_get_pointer(st->ht, sym->key, &found);
    
    //might have to have 3 different hash tables if I don't want to restructure hash table
    if (found && existing_sym) {
        //fprintf(stderr, "trying to insert symbol %s sym ns: %d, existing ns: %d\n", sym->key, sym->name_space, existing_sym->name_space);
        if (existing_sym->name_space == sym->name_space) {
            fprintf(stderr, "symbol %s already exists in namespace %s\n", sym->key, get_name_space(sym->name_space));
            return -1;
        }
    }

    switch(ht_insert(st->ht, sym->key, sym)) {
        case 0: 
            /*fprintf(stderr, "Inserted key %s into hash table successfully\n", sym->key); */
            return 0;
        case 1: 
            /*fprintf(stderr, "Error inserting into hash table: key %s already exists.\n", sym->key); */
            return -1;
        case 2: 
            /*fprintf(stderr, "Error inserting into hash table: rehashing failed\n"); */
            return -1;
        default: 
            /*fprintf(stderr, "Unknown return from hash table insert\n"); */
            return -1;
    }
}

/*void* ht_get_pointer(ht_t *ht, const char *key, bool *b);*/
SYMBOL* st_lookup(SYMTABLE* st, SCOPETYPE scope, char* key, NAMESPACE ns) {
    SYMBOL* sym = NULL;
    bool found = false;

    for (SYMTABLE* current = st; current != NULL; current = current->outer) {

        sym = (SYMBOL*) ht_get_pointer(current->ht, key, &found);

        if (found && sym && sym->name_space == ns) return sym;

        if (current->scope == FILE_SCOPE) break;
    }
    return NULL;
}


SYMBOL* st_new_symbol(char* key, ast_node_t* node, NAMESPACE ns, SYMTYPE type, STGCLASS stg_class, SYMTABLE* st, char* fname, int lineno) {

    SYMBOL* sym = (SYMBOL*) malloc(sizeof(SYMBOL));

    /*fprintf(stderr, "str type is: %d with key %s\n", key.type, key.string_literal); */

    sym->key = key;
    sym->name_space = ns;
    sym->type = type;
    sym->stg_class = stg_class;

    sym->file_name = strdup(fname);
    sym->line_num = lineno;
    sym->parent_sym = NULL;
    
    /*printf("key = %s, namespace = %d, type = %d, storage = %d\n", sym->key,sym->name_space, sym->type, sym->stg_class);*/
    sym->node = node;

    if(type == STRUCT_SYM || type == UNION_SYM) {
        sym->is_complete = 0;
        sym->mini_st = st_create(BLOCK_SCOPE, st);
    }

    return sym;
}
