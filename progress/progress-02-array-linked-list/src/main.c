#include <stdio.h>
#include <string.h>
#include "arena.h"
#include "array.h"
#include "linked_list.h"

void print_int(void *data) {
    printf("%d\n", *(int *)data);
}

void print_char(void *data) {
    printf("%c\n", *(char *)data);
}

int main(void) {
    Arena arena = {0};
    int ok;

    /* ===== Inisialisasi Arena ===== */
    printf("=== PROGRESS 2: ARRAY DAN LINKED LIST ===\n\n");
    printf("--- Inisialisasi Arena (512 byte) ---\n");
    ok = arena_init(&arena, 512);
    if (!ok) {
        printf("Gagal inisialisasi arena.\n");
        return 1;
    }

    printf("Arena berhasil diinisialisasi.\n");
    printf("Kondisi awal arena:\n");
    arena_dump(&arena, 16);

    /* ===== DEMONSTRASI ARRAY ===== */
    printf("\n\n--- DEMONSTRASI ARRAY ---\n");
    printf("Membuat Array untuk 5 elemen integer...\n");

    Array arr = {0};
    ok = array_create(&arena, &arr, sizeof(int), 5);
    if (!ok) {
        printf("Gagal membuat array.\n");
        return 1;
    }

    array_print_info(&arr);

    printf("\nMenambahkan 5 elemen (10, 20, 30, 40, 50)...\n");
    int values[] = {10, 20, 30, 40, 50};
    for (int i = 0; i < 5; i++) {
        array_append(&arena, &arr, &values[i]);
    }

    printf("Array setelah append:\n");
    array_print_info(&arr);

    printf("\nMembaca elemen array:\n");
    for (size_t i = 0; i < arr.count; i++) {
        int *val = (int *)array_get(&arena, &arr, i);
        if (val != NULL) {
            printf("  arr[%zu] (offset: %zu) = %d\n", i, arr.offset + (i * sizeof(int)), *val);
        }
    }

    printf("\nMengubah elemen arr[2] menjadi 999...\n");
    int new_val = 999;
    array_set(&arena, &arr, 2, &new_val);
    printf("arr[2] sekarang = %d\n", *(int *)array_get(&arena, &arr, 2));

    printf("\nKondisi arena setelah array:\n");
    arena_dump(&arena, 16);

    /* ===== DEMONSTRASI LINKED LIST ===== */
    printf("\n\n--- DEMONSTRASI LINKED LIST ---\n");
    printf("Membuat Linked List untuk elemen char...\n");

    LinkedList list = {0};
    ok = linked_list_create(&list, sizeof(char));
    if (!ok) {
        printf("Gagal membuat linked list.\n");
        return 1;
    }

    linked_list_print_info(&list);

    printf("\nMenambahkan elemen ke Linked List (append): A, B, C, D\n");
    char chars[] = {'A', 'B', 'C', 'D'};
    for (int i = 0; i < 4; i++) {
        linked_list_append(&arena, &list, &chars[i]);
    }

    linked_list_print_info(&list);

    printf("\nMelakukan traversal linked list:\n");
    linked_list_traverse(&arena, &list, print_char);

    printf("\nMenambahkan elemen ke awal (prepend): Z\n");
    char z = 'Z';
    linked_list_prepend(&arena, &list, &z);

    linked_list_print_info(&list);

    printf("\nTraversal setelah prepend:\n");
    linked_list_traverse(&arena, &list, print_char);

    printf("\nMengakses elemen dengan index:\n");
    for (size_t i = 0; i < list.count; i++) {
        char *c = (char *)linked_list_get(&arena, &list, i);
        if (c != NULL) {
            printf("  list[%zu] = %c\n", i, *c);
        }
    }

    printf("\nKondisi arena setelah linked list:\n");
    arena_dump(&arena, 16);

    /* ===== RESET DAN CLEANUP ===== */
    printf("\n\n--- RESET ARENA ---\n");
    arena_reset(&arena);
    printf("Kondisi arena setelah reset:\n");
    arena_dump(&arena, 16);

    arena_destroy(&arena);
    printf("\n=== PROGRAM SELESAI ===\n");

    return 0;
}
