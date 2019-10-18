#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "small_allocator.h"
#include "my_mmap.h"
#include "util.h"
#include "hash_map.h"

#define NB_GROUP_PAGE 6
#define MAX_SIZE sysconf(_SC_PAGESIZE)
#define FIRST_PAGE_SIZE 32

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
    item->first_shield = 0;
    item->second_shield = 0;
    if (current_size < MAX_SIZE)
        item->next = item + size_move;
    else
    {
        item->next = NULL;
        return;
    }

    struct freelist_item *current = item->next;
    while (current_size < MAX_SIZE)
    {
        current->prev = current - size_move;
        current->next = current + size_move;
        current->first_shield = 0;
        current->second_shield = 0;
        current = current->next;
        current_size += block_size;
    }
    current = current - size_move;
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

static struct block* allocate_new_block(size_t sub_block_size)
{
    struct block *block = NULL;
    init_block(sub_block_size, &block);
    return block;
}

void *allocate_item(struct small_allocator *small_allocator, size_t size)
{
    my_lock();

    struct block *head = small_allocator->heads[my_log(size)];
    if (head == NULL) //first malloc call with this size
    {
        head = init_block_start(small_allocator,
                    size,
                    &(small_allocator->heads[my_log(size)]));
    }
    if (head->beg_freelist == NULL) //page full
    {
        struct block *new_head = allocate_new_block(head->sub_block_size);
        small_allocator->heads[my_log(size)] = new_head;
        new_head->prev = small_allocator->heads[my_log(size)];
        new_head->next = head;
        head->prev = new_head;
        head->next = NULL;
        head = new_head;
    }

    struct freelist_item *first = head->beg_freelist;
    struct freelist_item *next = first->next;
    head->beg_freelist = next;
    if (next != NULL)
        next->prev = head;

    my_unlock();

    return first;
}

void init_small_allocator(void)
{
    g_small_allocator.mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    g_small_allocator.current = 0;

    //pthread_mutexattr_init(&g_small_allocator.attr);
    //pthread_mutexattr_settype(&g_small_allocator.attr, PTHREAD_MUTEX_RECURSIVE);
    //pthread_mutex_init(&g_small_allocator.mutex, &g_small_allocator.attr);

    my_lock();

    /*
    ** This if is to handle thread-safety
    ** I can't call the lock before because the mutex doesn't exist yet
    */
    if (g_small_allocator.page_size == 0)
    {
        g_small_allocator.page_size = sysconf(_SC_PAGESIZE);
        hash_init(&(g_small_allocator.map), NB_SLOTS);
        g_small_allocator.max_sub_block_size = g_small_allocator.page_size / 4;
        size_t size = FIRST_PAGE_SIZE;
        for (size_t i = 0; i < NB_GROUP_PAGE; i++)
        {
            g_small_allocator.size_item_per_block[i] = size;
            size <<= 1;
        }
    }

    my_unlock();
}