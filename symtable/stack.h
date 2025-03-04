#ifndef _STACK_H
#define _STACK_H

// can hold a max of 64 symtables allowed on stack
#define MAX_STACK_SIZE 64

#include <stdio.h>
#include <stdbool.h>
#include "symtable.h"

// code taken from my OS ps2q3 submission

typedef struct {
    SYMTABLE* arr[MAX_STACK_SIZE];
    int top;
} stack_t;

void stack_init(stack_t* stack);
bool stack_is_empty(stack_t* stack);
bool stack_is_full(stack_t* stack);
void stack_push(stack_t *stack, SYMTABLE* value);
SYMTABLE* stack_pop(stack_t* stack);
SYMTABLE* stack_peek(stack_t* stack); 

#endif
