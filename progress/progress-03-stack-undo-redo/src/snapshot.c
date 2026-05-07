#include "snapshot.h"
#include <string.h>

int snapshot_create(Arena *arena, Snapshot *snapshot, 
                   const void *data, size_t data_size, size_t elem_size) {
    if (arena == NULL || snapshot == NULL) {
        return 0;
    }
    
    // Calculate total size needed
    size_t total_size = data_size * elem_size;
    
    // Allocate space in arena
    void *data_ptr = arena_alloc(arena, total_size);
    if (data_ptr == NULL && total_size > 0) {
        return 0;
    }
    
    // Calculate offset
    size_t offset = (data_ptr != NULL) ? 
                   (size_t)((unsigned char *)data_ptr - arena->buffer) : 0;
    
    // Copy data if provided
    if (data != NULL && total_size > 0) {
        memcpy(data_ptr, data, total_size);
    }
    
    // Create snapshot
    snapshot->data_offset = offset;
    snapshot->data_size = data_size;
    snapshot->elem_size = elem_size;
    
    return 1;
}

int snapshot_restore(Arena *arena, Snapshot *snapshot, 
                    void *out_data, size_t *out_size) {
    if (arena == NULL || snapshot == NULL || out_data == NULL || out_size == NULL) {
        return 0;
    }
    
    void *snapshot_data = arena_get(arena, snapshot->data_offset);
    if (snapshot_data == NULL && snapshot->data_size > 0) {
        return 0;
    }
    
    size_t total_size = snapshot->data_size * snapshot->elem_size;
    
    // Copy data from snapshot
    if (snapshot_data != NULL && total_size > 0) {
        memcpy(out_data, snapshot_data, total_size);
    }
    
    *out_size = snapshot->data_size;
    return 1;
}

void *snapshot_get_data(Arena *arena, Snapshot *snapshot) {
    if (arena == NULL || snapshot == NULL) {
        return NULL;
    }
    return arena_get(arena, snapshot->data_offset);
}
