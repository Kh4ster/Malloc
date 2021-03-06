#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include "small_allocator.h"
#include "my_mmap.h"
#include "util.h"
#include "hash_map.h"

#define MAX_SIZE sysconf(_SC_PAGESIZE)
#define FIRST_PAGE_SIZE 16

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
    {
        item->next = NULL;
        return;
    }

    struct freelist_item *current = item->next;
    while (current_size < MAX_SIZE)
    {
        current->prev = current - size_move;
        current->next = current + size_move;
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
    if (*to_allocate == NULL) //no memory
        return NULL;
    struct block *block = *to_allocate;
    block->sub_block_size = size_sub_block;
    block->next = NULL;
    block->beg_freelist = block + 1;
    block->allocated_zones = 0;
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
    if (block == NULL)
        return NULL;
    block->prev = small_allocator;
    return block;
}

static struct block* allocate_new_block(size_t sub_block_size)
{
    struct block *block = NULL;
    return init_block(sub_block_size, &block);
}

static struct freelist_item* update_free_list(struct block *head)
{
    struct freelist_item *first = head->beg_freelist;
    struct freelist_item *next = first->next;
    ++head->allocated_zones;
    head->beg_freelist = next;
    if (next != NULL)
        next->prev = head;
    return first;
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
        if (head == NULL)
            return NULL;
    }
    else if (head->beg_freelist == NULL) //first page full
    {
        // Find the first free page
        while (head->next != NULL && head->beg_freelist == NULL)
            head = head->next;
        if (head->beg_freelist == NULL) // All pages are full
        {
            struct block *new_page = allocate_new_block(head->sub_block_size);
            if (new_page == NULL)
                return NULL;
            new_page->prev = head;
            new_page->next = NULL;
            head->next = new_page;
            head = new_page; // Set to head for further user
        }
    }

    //update free list
    struct freelist_item* first = update_free_list(head);

    my_unlock();

    return first;
}

void init_small_allocator(void)
{
    pthread_mutexattr_init(&g_small_allocator.attr);
    pthread_mutexattr_settype(&g_small_allocator.attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(&g_small_allocator.mutex, &g_small_allocator.attr);

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
