#include "stack.h"

void stack_init(stack_t* stack) {
    stack->top = -1;  // empty stack
}

bool stack_is_empty(stack_t* stack) {
    return stack->top == -1;
}

bool stack_is_full(stack_t* stack) {
    return stack->top == MAX_STACK_SIZE - 1;
}

void stack_push(stack_t* stack, void* value) {
    if (stack_is_full(stack)) {
        printf("stack_t overflow\n");
        return;
    }
    stack->arr[++stack->top] = value;
}

void* stack_pop(stack_t* stack) {
    if (stack_is_empty(stack)) {
        printf("stack_t is empty!\n");
        return NULL;
    }
    void* popped = stack->arr[stack->top];
    stack->top--;
    return popped;
}

void* stack_peek(stack_t* stack) {
    if (stack_is_empty(stack)) {
        printf("stack empty\n");
        return NULL;
    }
    return stack->arr[stack->top];
}
