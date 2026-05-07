#include "arena.h"
#include "stack.h"
#include "snapshot.h"
#include "session_manager.h"
#include <stdio.h>

int main() {
    printf("=== Progress 3: Stack + Data Stream (Undo/Redo Simulator) ===\n\n");
    
    // Initialize arena
    Arena arena;
    if (!arena_init(&arena, 512)) {
        printf("Failed to initialize arena\n");
        return 1;
    }
    
    printf("1. Arena initialized (512 bytes)\n");
    arena_dump(&arena, 64);
    
    // Initialize session manager
    SessionManager manager;
    if (!session_init(&arena, &manager, sizeof(int), 20)) {
        printf("Failed to initialize session\n");
        return 1;
    }
    
    printf("2. Session initialized (integer stack, max 20 elements)\n");
    session_print_info(&manager);
    
    // Simulate data stream operations
    printf("\n3. Simulating data stream operations:\n");
    
    printf("\n  [Op1] APPEND 10\n");
    session_append(&arena, &manager, 10);
    session_print_state(&manager);
    
    printf("\n  [Op2] APPEND 20\n");
    session_append(&arena, &manager, 20);
    session_print_state(&manager);
    
    printf("\n  [Op3] APPEND 30\n");
    session_append(&arena, &manager, 30);
    session_print_state(&manager);
    
    printf("\n  [Op4] APPEND 40\n");
    session_append(&arena, &manager, 40);
    session_print_state(&manager);
    
    printf("\n  Session info after 4 appends:\n");
    session_print_info(&manager);
    
    // Test undo operations
    printf("\n4. Testing UNDO operations:\n");
    
    printf("\n  [Undo1]\n");
    session_undo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  [Undo2]\n");
    session_undo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  Session info after 2 undos:\n");
    session_print_info(&manager);
    
    // Test redo operations
    printf("\n5. Testing REDO operations:\n");
    
    printf("\n  [Redo1]\n");
    session_redo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  Session info after 1 redo:\n");
    session_print_info(&manager);
    
    // New operation (should clear redo)
    printf("\n6. New operation after redo (APPEND 99):\n");
    printf("\n  [Op5] APPEND 99 (redo stack should clear)\n");
    session_append(&arena, &manager, 99);
    session_print_state(&manager);
    
    printf("\n  Session info after new append:\n");
    session_print_info(&manager);
    
    // Test remove operation
    printf("\n7. Testing REMOVE operation:\n");
    printf("\n  [Op6] REMOVE index 1\n");
    session_remove(&arena, &manager, 1);
    session_print_state(&manager);
    
    printf("\n  Session info after remove:\n");
    session_print_info(&manager);
    
    // Undo the remove
    printf("\n8. Undo the remove:\n");
    printf("\n  [Undo3]\n");
    session_undo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  Session info:\n");
    session_print_info(&manager);
    
    // Final arena dump
    printf("\n9. Final arena state:\n");
    arena_dump(&arena, 64);
    
    printf("\n=== Progress 3 Demo Complete ===\n");
    
    // Cleanup
    arena_destroy(&arena);
    
    return 0;
}
