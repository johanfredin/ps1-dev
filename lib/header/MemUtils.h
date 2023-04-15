#ifndef PSX_MEM_UTILS_H
#define PSX_MEM_UTILS_H

#define MEM_HEAP_START_ADDR 0x800F8000
#define MEM_HEAP_SIZE 0x00100000            // 1 mb heap

/**
 * Initialize ROM heap using default start addr and size.
 * Must be called before any memory allocation functions
 */
#define MEM_INIT_HEAP_3()                                            \
    InitHeap3((unsigned long *) MEM_HEAP_START_ADDR, MEM_HEAP_SIZE); \
    logr_log(INFO, "MemUtils.c", "MEM_INIT_HEAP_3", "ROM heap initialized. Start addr=%X, heap size=%X", MEM_HEAP_START_ADDR, MEM_HEAP_SIZE)

#define MEM_MALLOC_3(type) (type *) malloc3(sizeof(type))

#define MEM_MALLOC_3_CUS_SIZE(type, size) (type *)malloc3(size)
#define MEM_CALLOC_3_PTRS(n, t) (t **)calloc3((size_t)n, sizeof(t *))
#define MEM_MALLOC_3_STRS(n_chars) malloc3(sizeof(char *) * n_chars)
#define MEM_CALLOC_3(n, t) (t *)calloc3((size_t)n, sizeof(t))

#define MEM_FREE_3_AND_NULL(ptr) \
    if (ptr != NULL)             \
        free3(ptr);              \
    ptr = NULL

#define MEM_FREE_LINKED_LIST(linked_list_root, T)  \
    while (linked_list_root != NULL) {             \
        T *linked_list_aux = linked_list_root;     \
        linked_list_root = linked_list_root->next; \
        MEM_FREE_3_AND_NULL(linked_list_aux);      \
    }

#define MEM_FREE_STRS(char_array, n)                                \
    for (int str_idx = 0; str_idx < n; str_idx++) {                 \
        MEM_FREE_3_AND_NULL(char_array[i]);                         \
    }                                                               \
    MEM_FREE_3_AND_NULL(char_array)

/**
 * Create another entry in the linked list we are creating if the linked
 * list we are iterating has a next entry. This macro depends on both
 * ptr_to_check and curr_entry to be linked list entries with a *next pointer
 * property in them.
 * @param ptr_to_check the current entry in the linked list we are iterating
 * @param curr_entry the current entry in the linked list we are creating
 * @param ptr_type the type of the curr_entry
 */
#define MEM_MALLOC_3_AND_MOVE_TO_NEXT_IF_MORE_DATA(ptr_to_check, curr_entry, ptr_type) \
    if (ptr_to_check->next == NULL) {                                                  \
        curr_entry->next = NULL;                                                       \
    } else {                                                                           \
        curr_entry->next = MEM_MALLOC_3(ptr_type);                                     \
        curr_entry = curr_entry->next;                                                 \
    }

#endif // PSX_MEM_UTILS_H
