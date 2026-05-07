#include "arena.h"
#include <stdio.h>
#include <string.h>

static unsigned char ARENA_BUFFER[4096] = {0};

int arena_init(Arena *arena, size_t size) {
    if (arena == NULL || size == 0 || size > 4096) {
        return 0;
    }
    arena->buffer = ARENA_BUFFER;
    arena->capacity = size;
    arena->offset = 0;
    memset(arena->buffer, 0, size);
    return 1;
}

void *arena_alloc(Arena *arena, size_t size) {
    if (arena == NULL || size == 0) {
        return NULL;
    }
    if (arena->offset + size > arena->capacity) {
        return NULL;
    }
    void *ptr = arena->buffer + arena->offset;
    arena->offset += size;
    return ptr;
}

void *arena_get(Arena *arena, size_t offset) {
    if (arena == NULL || offset >= arena->capacity) {
        return NULL;
    }
    return arena->buffer + offset;
}

int arena_reset(Arena *arena) {
    if (arena == NULL) {
        return 0;
    }
    arena->offset = 0;
    memset(arena->buffer, 0, arena->capacity);
    return 1;
}

void arena_destroy(Arena *arena) {
    if (arena == NULL) {
        return;
    }
    arena->buffer = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}

size_t arena_available(Arena *arena) {
    if (arena == NULL) {
        return 0;
    }
    return arena->capacity - arena->offset;
}

void arena_dump(Arena *arena, size_t line_width) {
    if (arena == NULL || line_width == 0) {
        return;
    }
    printf("\n=== Dump Memori Arena ===\n");
    printf("Kapasitas: %zu byte, Terpakai: %zu byte, Tersedia: %zu byte\n",
           arena->capacity, arena->offset, arena_available(arena));
    printf("Breakdown Offset (lebar baris: %zu):\n", line_width);
    
    for (size_t i = 0; i < arena->capacity; i += line_width) {
        printf("[%04zu] ", i);
        for (size_t j = 0; j < line_width && i + j < arena->capacity; j++) {
            if (i + j < arena->offset) {
                printf("#");
            } else {
                printf(".");
            }
        }
        printf("\n");
    }
    printf("=========================\n\n");
}
