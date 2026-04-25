#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct {
    unsigned char *buffer;
    size_t capacity;
    size_t offset;
} Arena;

int arena_init(Arena *arena, size_t size);
int arena_alloc(Arena *arena, size_t size, size_t *out_offset);
void *arena_get(Arena *arena, size_t offset);
void arena_reset(Arena *arena);
void arena_destroy(Arena *arena);
size_t arena_available(const Arena *arena);
void arena_dump(const Arena *arena, size_t width);

#endif
