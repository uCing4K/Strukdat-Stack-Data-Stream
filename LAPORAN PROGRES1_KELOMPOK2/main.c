#include <stdio.h>
#include "arena.h"

int main() {
    Arena arena;
    arena_init(&arena, 32);

    printf("Kondisi Awal:\n");
    arena_dump(&arena, 8);

    int a = arena_alloc(&arena, 5);
    int b = arena_alloc(&arena, 10);

    printf("\nSetelah Alokasi:\n");
    arena_dump(&arena, 8);

    // isi data
    char *dataA = (char*) arena_get(&arena, a);
    for (int i = 0; i < 5; i++) dataA[i] = 'A';

    char *dataB = (char*) arena_get(&arena, b);
    for (int i = 0; i < 10; i++) dataB[i] = 'B';

    arena_reset(&arena);

    printf("\nSetelah reset:\n");
    arena_dump(&arena, 8);

    return 0;
}