#include "arena.h"
#include "stack.h"
#include "snapshot.h"
#include "session_manager.h"
#include <stdio.h>

int main() {
    printf("=== Progress 3: Stack + Data Stream (Simulator Undo/Redo) ===\n\n");
    
    // Initialize arena
    Arena arena;
    if (!arena_init(&arena, 512)) {
        printf("Gagal menginisialisasi arena\n");
        return 1;
    }
    
    printf("1. Arena diinisialisasi (512 bytes)\n");
    arena_dump(&arena, 64);
    
    // Initialize session manager
    SessionManager manager;
    if (!session_init(&arena, &manager, sizeof(int), 20)) {
        printf("Gagal menginisialisasi sesi\n");
        return 1;
    }
    
    printf("2. Sesi diinisialisasi (tumpukan integer, kapasitas max 20 elemen)\n");
    session_print_info(&manager);
    
    // Simulate data stream operations
    printf("\n3. Simulasi aliran data operasi:\n");
    
    printf("\n  [Op1] TAMBAH 10\n");
    session_append(&arena, &manager, 10);
    session_print_state(&manager);
    
    printf("\n  [Op2] TAMBAH 20\n");
    session_append(&arena, &manager, 20);
    session_print_state(&manager);
    
    printf("\n  [Op3] TAMBAH 30\n");
    session_append(&arena, &manager, 30);
    session_print_state(&manager);
    
    printf("\n  [Op4] TAMBAH 40\n");
    session_append(&arena, &manager, 40);
    session_print_state(&manager);
    
    printf("\n  Informasi sesi setelah 4 operasi tambah:\n");
    session_print_info(&manager);
    
    // Test undo operations
    printf("\n4. Pengujian operasi UNDO:\n");
    
    printf("\n  [Undo1]\n");
    session_undo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  [Undo2]\n");
    session_undo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  Informasi sesi setelah 2 undo:\n");
    session_print_info(&manager);
    
    // Test redo operations
    printf("\n5. Pengujian operasi REDO:\n");
    
    printf("\n  [Redo1]\n");
    session_redo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  Informasi sesi setelah 1 redo:\n");
    session_print_info(&manager);
    
    // New operation (should clear redo)
    printf("\n6. Operasi baru setelah redo (TAMBAH 99):\n");
    printf("\n  [Op5] TAMBAH 99 (stack redo akan dihapus)\n");
    session_append(&arena, &manager, 99);
    session_print_state(&manager);
    
    printf("\n  Informasi sesi setelah append baru:\n");
    session_print_info(&manager);
    
    // Test remove operation
    printf("\n7. Pengujian operasi HAPUS:\n");
    printf("\n  [Op6] HAPUS indeks 1\n");
    session_remove(&arena, &manager, 1);
    session_print_state(&manager);
    
    printf("\n  Informasi sesi setelah penghapusan:\n");
    session_print_info(&manager);
    
    // Undo the remove
    printf("\n8. Undo operasi penghapusan:\n");
    printf("\n  [Undo3]\n");
    session_undo(&arena, &manager);
    session_print_state(&manager);
    
    printf("\n  Informasi sesi:\n");
    session_print_info(&manager);
    
    // Final arena dump
    printf("\n9. Kondisi akhir arena:\n");
    arena_dump(&arena, 64);
    
    printf("\n=== Demonstrasi Progress 3 Selesai ===\n");
    
    // Cleanup
    arena_destroy(&arena);
    
    return 0;
}
