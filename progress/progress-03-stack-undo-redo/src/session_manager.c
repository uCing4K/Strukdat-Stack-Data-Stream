#include "session_manager.h"
#include <stdio.h>
#include <string.h>

int session_init(Arena *arena, SessionManager *manager, 
                size_t elem_size, size_t max_capacity) {
    if (arena == NULL || manager == NULL || elem_size == 0 || max_capacity == 0) {
        return 0;
    }
    
    // Allocate space for current state
    size_t state_size = max_capacity * elem_size;
    void *state_ptr = arena_alloc(arena, state_size);
    
    if (state_ptr == NULL) {
        return 0;
    }
    
    manager->current_state = (int *)state_ptr;
    manager->current_size = 0;
    manager->max_capacity = max_capacity;
    manager->elem_size = elem_size;
    
    // Initialize undo and redo stacks
    if (!stack_create(arena, &manager->undo_stack, sizeof(Snapshot))) {
        return 0;
    }
    
    if (!stack_create(arena, &manager->redo_stack, sizeof(Snapshot))) {
        return 0;
    }
    
    // Save initial empty state
    Snapshot initial;
    if (!snapshot_create(arena, &initial, NULL, 0, elem_size)) {
        return 0;
    }
    
    if (!stack_push(arena, &manager->undo_stack, &initial)) {
        return 0;
    }
    
    return 1;
}

int session_save_state(Arena *arena, SessionManager *manager) {
    if (arena == NULL || manager == NULL) {
        return 0;
    }
    
    // Create snapshot of current state
    Snapshot snapshot;
    if (!snapshot_create(arena, &snapshot, manager->current_state, 
                        manager->current_size, manager->elem_size)) {
        return 0;
    }
    
    // Push to undo stack
    if (!stack_push(arena, &manager->undo_stack, &snapshot)) {
        return 0;
    }
    
    return 1;
}

int session_append(Arena *arena, SessionManager *manager, int value) {
    if (arena == NULL || manager == NULL) {
        return 0;
    }
    
    if (manager->current_size >= manager->max_capacity) {
        return 0; // Full
    }
    
    // Save current state before modification
    if (!session_save_state(arena, manager)) {
        return 0;
    }
    
    // Add element
    manager->current_state[manager->current_size] = value;
    manager->current_size++;
    
    // Clear redo stack
    session_clear_redo(arena, manager);
    
    return 1;
}

int session_remove(Arena *arena, SessionManager *manager, size_t index) {
    if (arena == NULL || manager == NULL || index >= manager->current_size) {
        return 0;
    }
    
    // Save current state before modification
    if (!session_save_state(arena, manager)) {
        return 0;
    }
    
    // Remove element by shifting
    for (size_t i = index; i < manager->current_size - 1; i++) {
        manager->current_state[i] = manager->current_state[i + 1];
    }
    manager->current_size--;
    
    // Clear redo stack
    session_clear_redo(arena, manager);
    
    return 1;
}

int session_undo(Arena *arena, SessionManager *manager) {
    if (arena == NULL || manager == NULL) {
        return 0;
    }
    
    if (stack_is_empty(&manager->undo_stack)) {
        return 0;
    }
    
    // Save current state to redo
    Snapshot current;
    if (!snapshot_create(arena, &current, manager->current_state, 
                        manager->current_size, manager->elem_size)) {
        return 0;
    }
    
    if (!stack_push(arena, &manager->redo_stack, &current)) {
        return 0;
    }
    
    // Pop from undo
    Snapshot previous;
    if (!stack_pop(arena, &manager->undo_stack, &previous)) {
        return 0;
    }
    
    // Restore state
    if (!snapshot_restore(arena, &previous, manager->current_state, 
                         &manager->current_size)) {
        return 0;
    }
    
    return 1;
}

int session_redo(Arena *arena, SessionManager *manager) {
    if (arena == NULL || manager == NULL) {
        return 0;
    }
    
    if (stack_is_empty(&manager->redo_stack)) {
        return 0;
    }
    
    // Save current state to undo
    Snapshot current;
    if (!snapshot_create(arena, &current, manager->current_state, 
                        manager->current_size, manager->elem_size)) {
        return 0;
    }
    
    if (!stack_push(arena, &manager->undo_stack, &current)) {
        return 0;
    }
    
    // Pop from redo
    Snapshot next;
    if (!stack_pop(arena, &manager->redo_stack, &next)) {
        return 0;
    }
    
    // Restore state
    if (!snapshot_restore(arena, &next, manager->current_state, 
                         &manager->current_size)) {
        return 0;
    }
    
    return 1;
}

int session_clear_redo(Arena *arena, SessionManager *manager) {
    if (arena == NULL || manager == NULL) {
        return 0;
    }
    
    // Simply reset redo stack by reinitializing
    stack_create(arena, &manager->redo_stack, sizeof(Snapshot));
    return 1;
}

void session_print_state(SessionManager *manager) {
    if (manager == NULL) {
        return;
    }
    
    printf("Current state [%zu elements]: ", manager->current_size);
    if (manager->current_size == 0) {
        printf("[ ]");
    } else {
        printf("[ ");
        for (size_t i = 0; i < manager->current_size; i++) {
            printf("%d", manager->current_state[i]);
            if (i < manager->current_size - 1) printf(", ");
        }
        printf(" ]");
    }
    printf("\n");
}

void session_print_info(SessionManager *manager) {
    if (manager == NULL) {
        return;
    }
    
    printf("=== Session Info ===\n");
    printf("Undo stack size: %zu\n", stack_size(&manager->undo_stack));
    printf("Redo stack size: %zu\n", stack_size(&manager->redo_stack));
    session_print_state(manager);
    printf("====================\n");
}
