#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include "arena.h"
#include <stddef.h>

// Snapshot represents a saved state of the data
// For undo/redo, we store snapshots in stacks
typedef struct {
    size_t data_offset;    // Offset to data array in arena
    size_t data_size;      // Number of elements in data array
    size_t elem_size;      // Size of each element
} Snapshot;

// Create snapshot from current data
int snapshot_create(Arena *arena, Snapshot *snapshot, 
                   const void *data, size_t data_size, size_t elem_size);

// Restore snapshot to current data
int snapshot_restore(Arena *arena, Snapshot *snapshot, 
                    void *out_data, size_t *out_size);

// Get data from snapshot
void *snapshot_get_data(Arena *arena, Snapshot *snapshot);

#endif // SNAPSHOT_H
