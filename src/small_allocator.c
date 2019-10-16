#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "small_allocator.h"
#include "my_mmap.h"
#include "util.h"

#define NB_GROUP_PAGE 7
#define MAX_SIZE sysconf(_SC_PAGESIZE)

struct small_allocator small_allocator;

void *allocate_small_block(size_t size)
{
    struct block *head = small_allocator.heads[my_log(size)];
    struct freelist_item *first = head->beg_freelist; 
    struct freelist_item *next = first->next;
    head->beg_freelist = next;
    next->prev = head->beg_freelist;
    return first;
}

void init_free_list(struct block *block,
    struct freelist_item *item,
    size_t block_size
)
{
    /*
    ** Example : block_size = 16, sizeof(struct freelist_item) = 16
    ** Avec l'arithmétique des pointeurs on veut bien avancé de 16 / 16 = 1
    ** C'est à dire 1 struct freelist_item ou 16 bytes
    */
    size_t size_move = block_size / sizeof(struct freelist_item);
    assert(block != NULL);
    assert(item != NULL);
    /*
    ** Dans le cas 2048, on veut allouer que 1 donc cond doit péter direct
    ** Pour ça que *2
    */
    long current_size = (block_size * 2) + sizeof(struct block);
    item->prev = block;
    if (current_size < MAX_SIZE)
        item->next = item + size_move;
    else
        item->next = NULL;

    struct freelist_item *current = item->next;
    while (current_size < MAX_SIZE)
    {
        current->prev = current - size_move;
        current->next = current + size_move;
        current = current->next;
        current_size += block_size;
    }
}

void init_block(struct small_allocator *small_allocator,
    struct block *block,
    size_t block_size
)
{
    block->sub_block_size = block_size;
    block->prev = small_allocator;
    block->next = NULL;
    block->beg_freelist = block + 1;
    init_free_list(block, block->beg_freelist, block->sub_block_size);
}

void init_small_allocator(void)
{
    small_allocator.max_sub_block_size = sysconf(_SC_PAGESIZE) / 4;
    size_t size = 16;
    for (size_t i = 0; i < NB_GROUP_PAGE; i++)
    {
        small_allocator.heads[i] = my_mmap();
        init_block(&small_allocator, small_allocator.heads[i], size);
        size <<= 1;
    }
}