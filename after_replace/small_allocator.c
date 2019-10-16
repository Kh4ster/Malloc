#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#include "small_allocator.h"
#include "my_mmap.h"
#include "util.h"

#define NB_GROUP_PAGE 7
#define MAX_SIZE sysconf(_SC_PAGESIZE)
#define NB_SLOTS 256

struct small_allocator g_small_allocator = {0};

static void init_free_list(struct block *block,
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
    current->next = NULL;
}

/*
** Soit on veut allouer une nouvelle page à partir d'une liste existante
** Dans ce cas on connait déjà la taille des subblock
** Sinon c'est une première dans ce cas on nous passe la taille calculé
*/
static struct block* init_block(size_t size_sub_block,
    struct block **to_allocate
)
{
    *to_allocate = my_mmap();
    struct block *block = *to_allocate;
    block->sub_block_size = size_sub_block;
    block->next = NULL;
    block->beg_freelist = block + 1;
    init_free_list(block, block->beg_freelist, block->sub_block_size);
    return block;
}

struct block* init_block_start(struct small_allocator *small_allocator,
    size_t size,
    struct block **to_allocate
)
{
    size_t current_size = small_allocator->size_item_per_block[my_log(size)];
    struct block *block = init_block(current_size, to_allocate);
    block->prev = small_allocator;
    return block;
}

static struct block* allocate_new_block(struct block *prev)
{
    struct block *block = init_block(prev->sub_block_size,
        (struct block**)(&prev->next));
    block->prev = prev;
    prev->next = block;
    return block;
}

void *allocate_item(struct small_allocator *small_allocator, size_t size)
{
    struct block *head = small_allocator->heads[my_log(size)];
    if (head == NULL) //first malloc call with this size
    {
        head = init_block_start(small_allocator,
                    size,
                    &(small_allocator->heads[my_log(size)]));
    }
    struct block *last;
    //go to first none-full page or end to allocate a new one
    while (head != NULL && head->beg_freelist == NULL)
    {
        last = head;
        struct block *next = head->next;
        head = next;
    }

    if (head == NULL) //page full
        head = allocate_new_block(last);

    struct freelist_item *first = head->beg_freelist; 
    struct freelist_item *next = first->next;
    head->beg_freelist = next;
    if (next != NULL)
        next->prev = head->beg_freelist;
    return first;
}

void init_small_allocator(void)
{
    hash_init(&(g_small_allocator.map), NB_SLOTS);
    g_small_allocator.max_sub_block_size = sysconf(_SC_PAGESIZE) / 4;
    g_small_allocator.page_size = sysconf(_SC_PAGESIZE);
    size_t size = 16;
    for (size_t i = 0; i < 7; i++)
    {
        g_small_allocator.size_item_per_block[i] = size;
        size <<= 1;
    }
}