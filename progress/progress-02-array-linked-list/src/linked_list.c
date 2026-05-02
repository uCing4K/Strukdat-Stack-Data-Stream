#include "linked_list.h"

#include <stdio.h>
#include <string.h>

int linked_list_create(LinkedList *list, size_t elem_size) {
    if (list == NULL || elem_size == 0) {
        return 0;
    }

    list->head = LINKED_LIST_NULL;
    list->count = 0;
    list->elem_size = elem_size;
    return 1;
}

int linked_list_prepend(Arena *arena, LinkedList *list, const void *data) {
    if (arena == NULL || list == NULL || data == NULL) {
        return 0;
    }

    /* Alokasikan ruang untuk node: size_t (next_offset) + data */
    size_t node_size = sizeof(size_t) + list->elem_size;
    size_t node_offset;
    if (!arena_alloc(arena, node_size, &node_offset)) {
        return 0;
    }

    /* Dapatkan pointer ke node */
    size_t *next_offset_ptr = (size_t *)arena_get(arena, node_offset);
    if (next_offset_ptr == NULL) {
        return 0;
    }

    /* Set next pointer ke head lama */
    *next_offset_ptr = list->head;

    /* Salin data ke posisi setelah next_offset */
    void *data_ptr = (void *)((unsigned char *)next_offset_ptr + sizeof(size_t));
    memcpy(data_ptr, data, list->elem_size);

    /* Update head dan count */
    list->head = node_offset;
    list->count++;
    return 1;
}

int linked_list_append(Arena *arena, LinkedList *list, const void *data) {
    if (arena == NULL || list == NULL || data == NULL) {
        return 0;
    }

    /* Alokasikan ruang untuk node: size_t (next_offset) + data */
    size_t node_size = sizeof(size_t) + list->elem_size;
    size_t node_offset;
    if (!arena_alloc(arena, node_size, &node_offset)) {
        return 0;
    }

    /* Dapatkan pointer ke node */
    size_t *next_offset_ptr = (size_t *)arena_get(arena, node_offset);
    if (next_offset_ptr == NULL) {
        return 0;
    }

    /* Set next pointer ke NULL */
    *next_offset_ptr = LINKED_LIST_NULL;

    /* Salin data ke posisi setelah next_offset */
    void *data_ptr = (void *)((unsigned char *)next_offset_ptr + sizeof(size_t));
    memcpy(data_ptr, data, list->elem_size);

    /* Jika list kosong, set head */
    if (list->head == LINKED_LIST_NULL) {
        list->head = node_offset;
    } else {
        /* Cari node terakhir dan update next pointer nya */
        size_t current = list->head;
        size_t *current_next;
        while (1) {
            current_next = (size_t *)arena_get(arena, current);
            if (current_next == NULL) {
                return 0;
            }

            if (*current_next == LINKED_LIST_NULL) {
                break;
            }

            current = *current_next;
        }

        *current_next = node_offset;
    }

    list->count++;
    return 1;
}

void *linked_list_get(Arena *arena, const LinkedList *list, size_t index) {
    if (arena == NULL || list == NULL || index >= list->count) {
        return NULL;
    }

    size_t current = list->head;
    size_t i = 0;

    while (i < index && current != LINKED_LIST_NULL) {
        size_t *next_offset_ptr = (size_t *)arena_get(arena, current);
        if (next_offset_ptr == NULL) {
            return NULL;
        }

        current = *next_offset_ptr;
        i++;
    }

    if (current == LINKED_LIST_NULL) {
        return NULL;
    }

    /* Get data pointer: skip size_t (next_offset) */
    size_t *node_ptr = (size_t *)arena_get(arena, current);
    if (node_ptr == NULL) {
        return NULL;
    }

    return (void *)((unsigned char *)node_ptr + sizeof(size_t));
}

void linked_list_traverse(Arena *arena, const LinkedList *list, void (*print_func)(void *)) {
    if (arena == NULL || list == NULL) {
        return;
    }

    size_t current = list->head;
    size_t index = 0;

    printf("Traversal Linked List:\n");
    while (current != LINKED_LIST_NULL) {
        size_t *node_ptr = (size_t *)arena_get(arena, current);
        if (node_ptr == NULL) {
            break;
        }

        void *data_ptr = (void *)((unsigned char *)node_ptr + sizeof(size_t));
        printf("  Node[%zu] (offset: %zu): ", index, current);

        if (print_func != NULL) {
            print_func(data_ptr);
        }

        current = *node_ptr;
        index++;
    }
}

void linked_list_print_info(const LinkedList *list) {
    if (list == NULL) {
        return;
    }

    printf("LinkedList Info:\n");
    printf("  Head offset: %zu\n", (list->head == LINKED_LIST_NULL) ? -1 : list->head);
    printf("  Jumlah node: %zu\n", list->count);
    printf("  Ukuran data per node: %zu byte\n", list->elem_size);
}
