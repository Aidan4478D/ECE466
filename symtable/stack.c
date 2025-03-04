#include "stack.h"
#include "symtable.h"

void stack_init(stack_t* stack) {
    stack->top = -1;  // empty stack
}

bool stack_is_empty(stack_t* stack) {
    return stack->top == -1;
}

bool stack_is_full(stack_t *stack) {
    return stack->top == MAX_STACK_SIZE - 1;
}

void stack_push(stack_t* stack, SYMTABLE* value) {

    if(stack_is_full(stack)) {
        printf("stack_t overflow\n");
        return;
    }

    stack->arr[++stack->top] = value;
    //printf("Pushed %s onto the stack\n", value);
}

SYMTABLE* stack_pop(stack_t* stack) {

    if(stack_is_empty(stack)) {
        printf("stack_t is empty!\n");
        return NULL; 
    }

    SYMTABLE* popped = stack->arr[stack->top];
    stack->top--;

    //printf("Popped %s from the stack\n", popped);
    return popped;
}

SYMTABLE* stack_peek(stack_t* stack) {

    if(stack_is_empty(stack)) {
        printf("stack empty\n");
        return NULL;
    }
    return stack->arr[stack->top];
}
