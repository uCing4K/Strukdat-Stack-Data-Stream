#ifndef ARRAY_H
#define ARRAY_H

#include "arena.h"
#include <stddef.h>

typedef struct {
    size_t offset;      /* Offset di arena tempat data array dimulai */
    size_t count;       /* Jumlah elemen saat ini */
    size_t capacity;    /* Kapasitas maksimal elemen */
    size_t elem_size;   /* Ukuran setiap elemen dalam byte */
} Array;

/*
 * Membuat array di dalam arena
 * Parameters:
 *   - arena: pointer ke Arena
 *   - array: pointer ke Array yang akan diinisialisasi
 *   - elem_size: ukuran per elemen (misal: sizeof(int))
 *   - capacity: berapa banyak elemen yang ingin dialokasikan
 * Return: 1 jika sukses, 0 jika gagal
 */
int array_create(Arena *arena, Array *array, size_t elem_size, size_t capacity);

/*
 * Menambah elemen ke array
 * Parameters:
 *   - arena: pointer ke Arena
 *   - array: pointer ke Array
 *   - data: pointer ke data yang akan ditambahkan
 * Return: 1 jika sukses, 0 jika gagal (array penuh)
 */
int array_append(Arena *arena, Array *array, const void *data);

/*
 * Mendapatkan pointer ke elemen pada index
 * Parameters:
 *   - arena: pointer ke Arena
 *   - array: pointer ke Array
 *   - index: index elemen (0-based)
 * Return: pointer ke elemen, atau NULL jika out of bounds
 */
void *array_get(Arena *arena, const Array *array, size_t index);

/*
 * Mengatur nilai elemen pada index
 * Parameters:
 *   - arena: pointer ke Arena
 *   - array: pointer ke Array
 *   - index: index elemen (0-based)
 *   - data: pointer ke data yang akan disalin
 * Return: 1 jika sukses, 0 jika gagal
 */
int array_set(Arena *arena, const Array *array, size_t index, const void *data);

/*
 * Menampilkan informasi array
 */
void array_print_info(const Array *array);

#endif
