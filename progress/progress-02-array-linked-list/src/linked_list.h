#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "arena.h"
#include <stddef.h>

/*
 * Linked List berbasis offset
 * 
 * Setiap node memiliki struktur:
 *   - next_offset (size_t): offset ke node berikutnya, atau LINKED_LIST_NULL untuk akhir
 *   - data: elemen data (ukuran dinamis)
 */

#define LINKED_LIST_NULL ((size_t)-1)

typedef struct {
    size_t head;        /* Offset ke node pertama, atau LINKED_LIST_NULL jika kosong */
    size_t count;       /* Jumlah node saat ini */
    size_t elem_size;   /* Ukuran data di setiap node */
} LinkedList;

/*
 * Membuat linked list di dalam arena
 * Parameters:
 *   - arena: pointer ke Arena
 *   - list: pointer ke LinkedList yang akan diinisialisasi
 *   - elem_size: ukuran per elemen (misal: sizeof(int))
 * Return: 1 jika sukses, 0 jika gagal
 */
int linked_list_create(LinkedList *list, size_t elem_size);

/*
 * Menambah elemen ke awal linked list
 * Parameters:
 *   - arena: pointer ke Arena
 *   - list: pointer ke LinkedList
 *   - data: pointer ke data yang akan ditambahkan
 * Return: 1 jika sukses, 0 jika gagal
 */
int linked_list_prepend(Arena *arena, LinkedList *list, const void *data);

/*
 * Menambah elemen ke akhir linked list
 * Parameters:
 *   - arena: pointer ke Arena
 *   - list: pointer ke LinkedList
 *   - data: pointer ke data yang akan ditambahkan
 * Return: 1 jika sukses, 0 jika gagal
 */
int linked_list_append(Arena *arena, LinkedList *list, const void *data);

/*
 * Mendapatkan pointer ke data di posisi index
 * Parameters:
 *   - arena: pointer ke Arena
 *   - list: pointer ke LinkedList
 *   - index: posisi (0-based)
 * Return: pointer ke data, atau NULL jika out of bounds
 */
void *linked_list_get(Arena *arena, const LinkedList *list, size_t index);

/*
 * Melakukan traversal dan menampilkan semua elemen
 * Parameters:
 *   - arena: pointer ke Arena
 *   - list: pointer ke LinkedList
 */
void linked_list_traverse(Arena *arena, const LinkedList *list, void (*print_func)(void *));

/*
 * Menampilkan informasi linked list
 */
void linked_list_print_info(const LinkedList *list);

#endif
