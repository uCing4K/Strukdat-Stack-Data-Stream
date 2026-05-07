#ifndef STACK_H
#define STACK_H

#include "arena.h"
#include <stddef.h>

typedef struct {
    size_t head;           // Offset to top of stack (or -1 if empty)
    size_t count;          // Number of elements in stack
    size_t elem_size;      // Size of each element
} Stack;

#define STACK_NULL ((size_t)-1)

// Stack operations
int stack_create(Arena *arena, Stack *stack, size_t elem_size);
int stack_push(Arena *arena, Stack *stack, const void *data);
int stack_pop(Arena *arena, Stack *stack, void *out_data);
int stack_peek(Arena *arena, Stack *stack, void *out_data);
int stack_is_empty(Stack *stack);
size_t stack_size(Stack *stack);

// Debugging
void stack_traverse(Arena *arena, Stack *stack);

#endif // STACK_H
