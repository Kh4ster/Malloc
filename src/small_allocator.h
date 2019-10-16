#ifndef SMALL_ALLOCATOR_H
#define SMALL_ALLOCATOR_H

#include <stddef.h>

#define NB_GROUP_PAGE 7

extern struct small_allocator small_allocator;

//Page découpé en bloc de soit 16 32 64 128 256 512 1024 bytes
struct small_allocator
{
    //tableau de pointeur vers les dlist de page découpé en free-list
    struct block *heads[NB_GROUP_PAGE];
    size_t max_sub_block_size;
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

void *allocate_small_block(size_t size);
void init_free_list(struct block *block,
    struct freelist_item *item,
    size_t block_size);

void init_block(struct small_allocator *small_allocator,
    struct block *block,
    size_t block_size);

void init_small_allocator(void);


#endif /* !SMALL_ALLOCATOR_H */