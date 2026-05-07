#include "stack.h"
#include <stdio.h>
#include <string.h>

int stack_create(Arena *arena, Stack *stack, size_t elem_size) {
    if (arena == NULL || stack == NULL || elem_size == 0) {
        return 0;
    }
    stack->head = STACK_NULL;
    stack->count = 0;
    stack->elem_size = elem_size;
    return 1;
}

int stack_push(Arena *arena, Stack *stack, const void *data) {
    if (arena == NULL || stack == NULL || data == NULL) {
        return 0;
    }
    
    // Node layout: [size_t next_offset (8 byte)][data (elem_size byte)]
    size_t node_size = sizeof(size_t) + stack->elem_size;
    void *node_ptr = arena_alloc(arena, node_size);
    
    if (node_ptr == NULL) {
        return 0;
    }
    
    // Calculate node offset
    size_t node_offset = (size_t)((unsigned char *)node_ptr - arena->buffer);
    
    // Set next pointer
    size_t *next_ptr = (size_t *)node_ptr;
    *next_ptr = stack->head;
    
    // Copy data
    void *data_ptr = (unsigned char *)node_ptr + sizeof(size_t);
    memcpy(data_ptr, data, stack->elem_size);
    
    // Update stack head
    stack->head = node_offset;
    stack->count++;
    
    return 1;
}

int stack_pop(Arena *arena, Stack *stack, void *out_data) {
    if (arena == NULL || stack == NULL) {
        return 0;
    }
    
    if (stack_is_empty(stack)) {
        return 0;
    }
    
    // Get node at head
    void *node_ptr = arena_get(arena, stack->head);
    if (node_ptr == NULL) {
        return 0;
    }
    
    // Extract data
    if (out_data != NULL) {
        void *data_ptr = (unsigned char *)node_ptr + sizeof(size_t);
        memcpy(out_data, data_ptr, stack->elem_size);
    }
    
    // Get next offset
    size_t *next_ptr = (size_t *)node_ptr;
    size_t next_offset = *next_ptr;
    
    // Update stack head
    stack->head = next_offset;
    stack->count--;
    
    return 1;
}

int stack_peek(Arena *arena, Stack *stack, void *out_data) {
    if (arena == NULL || stack == NULL || out_data == NULL) {
        return 0;
    }
    
    if (stack_is_empty(stack)) {
        return 0;
    }
    
    // Get node at head
    void *node_ptr = arena_get(arena, stack->head);
    if (node_ptr == NULL) {
        return 0;
    }
    
    // Extract data
    void *data_ptr = (unsigned char *)node_ptr + sizeof(size_t);
    memcpy(out_data, data_ptr, stack->elem_size);
    
    return 1;
}

int stack_is_empty(Stack *stack) {
    if (stack == NULL) {
        return 1;
    }
    return stack->head == STACK_NULL;
}

size_t stack_size(Stack *stack) {
    if (stack == NULL) {
        return 0;
    }
    return stack->count;
}

void stack_traverse(Arena *arena, Stack *stack) {
    if (arena == NULL || stack == NULL) {
        return;
    }
    
    if (stack_is_empty(stack)) {
        printf("  [Empty stack]\n");
        return;
    }
    
    printf("  Stack contents (top to bottom):\n");
    size_t current = stack->head;
    int index = 0;
    
    while (current != STACK_NULL) {
        void *node_ptr = arena_get(arena, current);
        if (node_ptr == NULL) {
            break;
        }
        
        // Print data as integer (assuming integer stack for demo)
        void *data_ptr = (unsigned char *)node_ptr + sizeof(size_t);
        int *int_data = (int *)data_ptr;
        printf("    [%d] = %d\n", index, *int_data);
        
        // Move to next
        size_t *next_ptr = (size_t *)node_ptr;
        current = *next_ptr;
        index++;
    }
}
