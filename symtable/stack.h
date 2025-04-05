#ifndef _STACK_H
#define _STACK_H

// max elements allowd in stack
#define MAX_STACK_SIZE 64

#include <stdio.h>
#include <stdbool.h>

// stack that holds void* pointers
typedef struct {
    void* arr[MAX_STACK_SIZE];
    int top;
} stack_t;

void stack_init(stack_t* stack);
bool stack_is_empty(stack_t* stack);
bool stack_is_full(stack_t* stack);
void stack_push(stack_t* stack, void* value);
void* stack_pop(stack_t* stack);
void* stack_peek(stack_t* stack);

#endif
