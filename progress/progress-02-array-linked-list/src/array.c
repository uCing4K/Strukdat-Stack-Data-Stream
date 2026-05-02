#include "array.h"

#include <stdio.h>
#include <string.h>

int array_create(Arena *arena, Array *array, size_t elem_size, size_t capacity) {
    if (arena == NULL || array == NULL || elem_size == 0 || capacity == 0) {
        return 0;
    }

    size_t total_size = elem_size * capacity;
    if (!arena_alloc(arena, total_size, &array->offset)) {
        return 0;
    }

    array->elem_size = elem_size;
    array->capacity = capacity;
    array->count = 0;
    return 1;
}

int array_append(Arena *arena, Array *array, const void *data) {
    if (arena == NULL || array == NULL || data == NULL) {
        return 0;
    }

    if (array->count >= array->capacity) {
        return 0;
    }

    /* Hitung offset elemen yang akan ditambahkan */
    size_t elem_offset = array->offset + (array->count * array->elem_size);
    void *slot = arena_get(arena, elem_offset);
    if (slot == NULL) {
        return 0;
    }

    memcpy(slot, data, array->elem_size);
    array->count++;
    return 1;
}

void *array_get(Arena *arena, const Array *array, size_t index) {
    if (arena == NULL || array == NULL || index >= array->count) {
        return NULL;
    }

    size_t elem_offset = array->offset + (index * array->elem_size);
    return arena_get(arena, elem_offset);
}

int array_set(Arena *arena, const Array *array, size_t index, const void *data) {
    if (arena == NULL || array == NULL || data == NULL || index >= array->count) {
        return 0;
    }

    void *slot = array_get(arena, array, index);
    if (slot == NULL) {
        return 0;
    }

    memcpy(slot, data, array->elem_size);
    return 1;
}

void array_print_info(const Array *array) {
    if (array == NULL) {
        return;
    }

    printf("Array Info:\n");
    printf("  Offset dalam arena: %zu\n", array->offset);
    printf("  Elemen saat ini: %zu\n", array->count);
    printf("  Kapasitas: %zu\n", array->capacity);
    printf("  Ukuran per elemen: %zu byte\n", array->elem_size);
    printf("  Total memori terpakai: %zu byte\n", array->count * array->elem_size);
}
