#ifndef ARENA_H
#define ARENA_H

#include <stddef.h>

typedef struct {
    unsigned char *buffer;
    size_t capacity;
    size_t offset;
} Arena;

int arena_init(Arena *arena, size_t size);
void *arena_alloc(Arena *arena, size_t size);
void *arena_get(Arena *arena, size_t offset);
int arena_reset(Arena *arena);
void arena_destroy(Arena *arena);
size_t arena_available(Arena *arena);
void arena_dump(Arena *arena, size_t line_width);

#endif // ARENA_H
