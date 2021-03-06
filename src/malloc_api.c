#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "small_allocator.h"
#include "my_mmap.h"
#include "util.h"
#include "hash_map.h"

static void* allocate_big_block(struct hash_map *map, size_t size)
{
    void *ptr = my_mmap_size(size);
    if (ptr == NULL)
        return NULL;

    my_lock();

    hash_insert(map, ptr, size);

    my_unlock();

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
            return allocate_big_block(&(g_small_allocator.map), size);
    }
    return NULL;
}

static void free_big_block(struct hash_map *map, void *ptr, size_t size)
{
    hash_remove(map, ptr);
    my_munmap(ptr, size);
}

static void free_empty_page(struct block *head)
{
    void *prev_ptr = head->prev;
    if (prev_ptr != &g_small_allocator) // Block in the middle of the chain
    {
        struct block *prev = prev_ptr;
        prev->next = head->next;
        if (head->next != NULL)
        {
            struct block* next = head->next;
            next->prev = prev;
        }
    }
    else // It's the head of a bucket
    {
        g_small_allocator.heads[my_log(head->sub_block_size)] = head->next;
        if (head->next != NULL)
        {
            struct block* next = head->next;
            next->prev = &g_small_allocator;
        }
    }
    my_munmap(head, g_small_allocator.page_size);
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
    else //if he is lower than the first free block
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
    --head->allocated_zones;
    if (head->allocated_zones == 0) // Page totally empty
        free_empty_page(head);
}

void my_free(void *ptr)
{
    if (ptr == NULL)
        return;

    my_lock();

    if (g_small_allocator.page_size == 0)
        init_small_allocator();

    size_t size = hash_find(&(g_small_allocator.map), ptr);
    if (size == 0)
        free_small_block(ptr);
    else
        free_big_block(&(g_small_allocator.map), ptr, size);

    my_unlock();
}

/*
** already thread-sage cause my_malloc is thread safe
** i don't see a situation in which malloc would be called by 2 threads
** and return the same adress
*/
void *my_calloc(size_t number, size_t size)
{
    size_t a;
    if (__builtin_umull_overflow(number, size, &a))
        return NULL;

    void *ptr = my_malloc(number * size);
    if (ptr == NULL)
        return NULL;
    memset(ptr, 0, number * size);
    return ptr;
}

void *realloc_big_block(void *ptr,
                        struct hash_slot *slot,
                        size_t new_size
)
{
    const int page_size = g_small_allocator.page_size;
    void *new_ptr;
    if (new_size / page_size > slot->size / page_size)
    {
        new_ptr = my_malloc(new_size);
        if (new_ptr == NULL)
            return NULL;
        memcpy(new_ptr, ptr, slot->size);
        my_free(ptr);
        return new_ptr;
    }
    else
    {
        my_lock();

        slot->size = new_size;

        my_unlock();

        return ptr;
    }
}

// already thread-safe cause my_malloc and my_free are
static void* realloc_small_block(void *ptr, size_t new_size)
{
    struct block *head = get_page_address(ptr);
    if (head->sub_block_size >= new_size)
        return ptr;
    void *new_ptr = my_malloc(new_size);
    if (new_ptr == NULL)
        return NULL;
    memcpy(new_ptr, ptr, head->sub_block_size);
    my_free(ptr);
    return new_ptr;
}

void *my_realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return my_malloc(size);
    else if (size == 0)
    {
        my_free(ptr);
        return NULL;
    }

    my_lock();

    if (g_small_allocator.page_size == 0)
        init_small_allocator();

    struct hash_slot *slot = hash_get_slot(&(g_small_allocator.map), ptr);

    my_unlock();

    if (slot == NULL)
        return realloc_small_block(ptr, size);
    else
        return realloc_big_block(ptr, slot, size);
}
