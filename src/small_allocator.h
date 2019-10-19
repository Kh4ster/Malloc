#ifndef SMALL_ALLOCATOR_H
#define SMALL_ALLOCATOR_H

#include <stddef.h>
#include <pthread.h>

#include "hash_map.h"

#define NB_GROUP_PAGE 7

extern struct small_allocator g_small_allocator;

//Page découpé en bloc de soit 16 32 64 128 256 512 1024 bytes
struct small_allocator
{
    //tableau de pointeur vers les dlist de page découpé en free-list
    struct block *heads[NB_GROUP_PAGE];
    size_t max_sub_block_size;
    int page_size;
    size_t size_item_per_block[NB_GROUP_PAGE];
    struct hash_map map;
    pthread_mutex_t mutex;
    //only used for malloc recursive call to malloc in the hashmap
    pthread_t current;
    pthread_mutexattr_t attr;
};

//Au début de ma page pour savoir où commence ma freelist et où sont les atr pag
struct block
{
    //pointeur vers prochain bloc avec une freelist composé de bloc de mm taill
    void *next;
    void *prev;
    //la partie donné dans laquelle on aura notre freelist
    void *beg_freelist;
    //taile des sous block de la free_list en bytes
    size_t sub_block_size;
};

struct freelist_item
{
    void *next;
    void *prev;
};

void *allocate_item(struct small_allocator *small_allocator, size_t size);
struct block* init_block_start(struct small_allocator *small_allocator,
    size_t size,
    struct block **to_allocate);
void init_small_allocator(void);


#endif /* !SMALL_ALLOCATOR_H */
