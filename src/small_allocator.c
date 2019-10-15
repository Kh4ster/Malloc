#include "small_allocator.h"
#include "my_mmap.h"

#define NB_GROUP_PAGE 7
#define MAX_SIZE 4096

struct small_allocator small_allocator;

void init_free_list(struct block *block,
    struct freelist_item *item,
    size_t block_size
)
{
    /*
    ** Dans le cas 2048, on veut allouer que 1 donc cond doit péter direct
    ** Pour ça que *2
    */
    size_t current_size = (block_size * 2) + sizeof(struct block);
    item->prev = block;
    if (current_size < MAX_SIZE)
        item->next = &item + block_size;
    else
        item->next = NULL;

    struct freelist_item *current = item->next;
    while (current_size < MAX_SIZE) //<= ?
    {
        current->prev = &current - block_size;
        current->next = &current + block_size;
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
    block->beg_freelist = block + sizeof(struct block);
    init_free_list(block, block->beg_freelist, block->sub_block_size);
}

//On manipule la variable gloabale que dans cette fonction
void init_small_allocator(void)
{
    size_t size = 16;
    for (size_t i = 0; i < NB_GROUP_PAGE; i++)
    {
        small_allocator.heads[0] = my_mmap();
        init_block(&small_allocator, small_allocator.heads[0], size);
        size <<= 1;
    }
}