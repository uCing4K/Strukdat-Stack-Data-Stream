#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "arena.h"
#include "stack.h"
#include "snapshot.h"
#include <stddef.h>

typedef struct {
    Stack undo_stack;
    Stack redo_stack;
    int *current_state;        // Pointer to current data
    size_t current_size;       // Current number of elements
    size_t max_capacity;       // Max elements allowed
    size_t elem_size;          // Size per element
} SessionManager;

// Initialize session
int session_init(Arena *arena, SessionManager *manager, 
                size_t elem_size, size_t max_capacity);

// Append element to current state
int session_append(Arena *arena, SessionManager *manager, int value);

// Remove element at index from current state
int session_remove(Arena *arena, SessionManager *manager, size_t index);

// Undo operation
int session_undo(Arena *arena, SessionManager *manager);

// Redo operation
int session_redo(Arena *arena, SessionManager *manager);

// Save current state to undo stack
int session_save_state(Arena *arena, SessionManager *manager);

// Clear redo stack (called after new operation)
int session_clear_redo(Arena *arena, SessionManager *manager);

// Get current state
void session_print_state(SessionManager *manager);

// Get undo/redo stack info
void session_print_info(SessionManager *manager);

#endif // SESSION_MANAGER_H
