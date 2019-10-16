#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "small_allocator.h"
#include "my_mmap.h"
#include "util.h"
#include "hash_map.h"

static void* allocate_big_block(struct hash_map *map, size_t size)
{
    void *ptr = my_mmap_size(size); 
    hash_insert(map, ptr, size);
    return ptr;
}

void *my_malloc(size_t size)
{
    if (g_small_allocator.max_sub_block_size == 0)
        init_small_allocator();
    if (size > 0)
    {
        if (size <= g_small_allocator.max_sub_block_size)
            return allocate_item(&g_small_allocator, size);
        else
            return allocate_big_block(&(g_small_allocator.map),  size);
    }
    return NULL;
}

//not done, will use hashmap
static void free_big_block(struct hash_map *map, void *ptr, size_t size)
{
    hash_remove(map, ptr);
    my_munmap(ptr, size);
}

static void free_small_block(void *ptr)
{
    struct block *head = get_page_address(ptr);
    /*
    ** if block is higher than current top or no top
    ** should now become the first
    */
    if (head->beg_freelist == NULL || ptr < head->beg_freelist)
    {
        struct freelist_item *current = ptr;
        struct freelist_item *old_top = head->beg_freelist;
        current->prev = head;
        current->next = old_top;
        head->beg_freelist = current;
        if (old_top != NULL)
            old_top->prev = current;
    }
    else
    {
        struct freelist_item *current = ptr;
        struct freelist_item *top = head->beg_freelist;
        current->prev = top;
        struct freelist_item *old_next = top->next;
        current->next = old_next;
        top->next = current;
        if (old_next != NULL)
            old_next->prev = current;
    }
}

void my_free(void *ptr)
{
    if (ptr == NULL)
        return;
    size_t size = hash_find(&(g_small_allocator.map), ptr);
    if (size == 0)
        free_small_block(ptr);
    else
        free_big_block(&(g_small_allocator.map), ptr, size);
}

void *my_calloc(size_t number, size_t size)
{
    size_t a;
    if (__builtin_umull_overflow(number, size, &a))
        return NULL;
    void *ptr = my_malloc(number * size);
    memset(ptr, 0, number * size);
    return ptr;
}

void *realloc(void *ptr, size_t size);