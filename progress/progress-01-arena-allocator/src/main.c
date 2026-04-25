#include <stdio.h>

#include "arena.h"

int main(void) {
    Arena arena = {0};
    size_t offset_a = 0;
    size_t offset_b = 0;
    size_t offset_c = 0;
    int ok;
    size_t i;

    ok = arena_init(&arena, 32);
    if (!ok) {
        printf("Gagal inisialisasi arena.\n");
        return 1;
    }

    printf("Kondisi awal arena:\n");
    arena_dump(&arena, 8);

    ok = arena_alloc(&arena, 6, &offset_a);
    if (ok) {
        unsigned char *data_a = (unsigned char *)arena_get(&arena, offset_a);
        for (i = 0; i < 6; i++) {
            data_a[i] = 'A';
        }
        printf("\nAlokasi blok A (6 byte) berhasil di offset %zu.\n", offset_a);
    }

    ok = arena_alloc(&arena, 10, &offset_b);
    if (ok) {
        unsigned char *data_b = (unsigned char *)arena_get(&arena, offset_b);
        for (i = 0; i < 10; i++) {
            data_b[i] = 'B';
        }
        printf("Alokasi blok B (10 byte) berhasil di offset %zu.\n", offset_b);
    }

    ok = arena_alloc(&arena, 20, &offset_c);
    if (ok) {
        printf("Alokasi blok C (20 byte) berhasil di offset %zu.\n", offset_c);
    } else {
        printf("Alokasi blok C (20 byte) gagal karena kapasitas tidak cukup.\n");
    }

    printf("\nKondisi arena setelah alokasi:\n");
    arena_dump(&arena, 8);

    arena_reset(&arena);
    printf("\nSetelah arena_reset():\n");
    arena_dump(&arena, 8);

    arena_destroy(&arena);
    return 0;
}
