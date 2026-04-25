#include <stdio.h>
#include <stdlib.h>
#include "arena.h"

void arena_init(Arena *arena, size_t size) {
    arena->buffer = (unsigned char*) malloc(size);
    arena->capacity = size;
    arena->offset = 0;
}

int arena_alloc(Arena *arena, size_t size) {
    if (arena->offset + size > arena->capacity) {
        return -1; // gagal
    }

    int allocated_offset = arena->offset;
    arena->offset += size;

    return allocated_offset;
}

void* arena_get(Arena *arena, int offset) {
    if (offset < 0 || offset >= arena->capacity) {
        return NULL;
    }

    return (void*)(arena->buffer + offset);
}

void arena_reset(Arena *arena) {
    arena->offset = 0;
}

void arena_dump(Arena *arena, int width) {
    printf("\n=== ARENA DUMP ===\n");

    for (size_t i = 0; i < arena->capacity; i++) {
        if (i % width == 0) printf("\n");

        if (i < arena->offset)
            printf("[#]");
        else
            printf("[.]");
    }

    printf("\n\nOffset: %zu / %zu\n", arena->offset, arena->capacity);
}