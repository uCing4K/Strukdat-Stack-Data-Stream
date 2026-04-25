#ifndef ARENA_H
#define ARENA_H

#include <stdio.h>

typedef struct {
    unsigned char *buffer;
    size_t capacity;
    size_t offset;
} Arena;

void arena_init(Arena *arena, size_t size);
int arena_alloc(Arena *arena, size_t size);
void* arena_get(Arena *arena, int offset);
void arena_reset(Arena *arena);
void arena_dump(Arena *arena, int width);

#endif