#include "arena.h"

#include <stdio.h>

#define ARENA_MAX_CAPACITY 4096

static unsigned char arena_storage[ARENA_MAX_CAPACITY];

int arena_init(Arena *arena, size_t size) {
    if (arena == NULL || size == 0) {
        return 0;
    }

    if (size > ARENA_MAX_CAPACITY) {
        return 0;
    }

    arena->buffer = arena_storage;
    arena->capacity = size;
    arena->offset = 0;
    return 1;
}

int arena_alloc(Arena *arena, size_t size, size_t *out_offset) {
    if (arena == NULL || out_offset == NULL || size == 0) {
        return 0;
    }

    if (arena->offset + size > arena->capacity) {
        return 0;
    }

    *out_offset = arena->offset;
    arena->offset += size;
    return 1;
}

void *arena_get(Arena *arena, size_t offset) {
    if (arena == NULL || arena->buffer == NULL || offset >= arena->capacity) {
        return NULL;
    }

    return (void *)(arena->buffer + offset);
}

void arena_reset(Arena *arena) {
    if (arena == NULL) {
        return;
    }

    arena->offset = 0;
}

void arena_destroy(Arena *arena) {
    if (arena == NULL) {
        return;
    }

    arena->buffer = NULL;
    arena->capacity = 0;
    arena->offset = 0;
}

size_t arena_available(const Arena *arena) {
    if (arena == NULL || arena->capacity < arena->offset) {
        return 0;
    }

    return arena->capacity - arena->offset;
}

void arena_dump(const Arena *arena, size_t width) {
    size_t i;

    if (arena == NULL || width == 0) {
        return;
    }

    printf("\n=== ARENA DUMP ===\n");
    for (i = 0; i < arena->capacity; i++) {
        if (i % width == 0) {
            printf("\n");
        }

        if (i < arena->offset) {
            printf("[#]");
        } else {
            printf("[.]");
        }
    }

    printf("\n\nOffset: %zu / %zu\n", arena->offset, arena->capacity);
    printf("Tersisa: %zu byte\n", arena_available(arena));
}
