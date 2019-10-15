#ifndef SMALL_ALLOCATOR_H
#define SMALL_ALLOCATOR_H

#include <stddef.h>

#define NB_GROUP_PAGE 7

//Page découpé en bloc de soit 16 32 64 128 256 512 1024
struct small_allocator
{
    //tableau de pointeur vers les dlist de page découpé en free-list
    struct block *heads[NB_GROUP_PAGE];
};

//Au début de ma page pour savoir où commence ma freelist et où sont les atr pag
struct block
{
    //pointeur vers prochain bloc avec une freelist composé de bloc de mm taill
    void *next;
    void *prev;
    //la partie donné dans laquelle on aura notre freelist
    void *beg_freelist;
    size_t sub_block_size;
};

struct freelist_item
{
    void *next;
    void *prev;
};


#endif /* !SMALL_ALLOCATOR_H */