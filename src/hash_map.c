#include <stdio.h>
#include <assert.h>

#include "hash_map.h"
#include "malloc_api.h"
#include "my_mmap.h"

static size_t hash(long long key, size_t table_size)
{
    return key % table_size;
}

static struct hash_slot* get_slot(struct hash_map *set, void *value)
{
    size_t v = hash((long long)value, set->size);
    struct hash_slot *s = &(set->slots[v]);
    return s;
}

void hash_insert(struct hash_map *set, void *value, size_t size)
{
    assert(value != NULL);
    struct hash_slot *s = get_slot(set, value);
    if (s->data == NULL) //first time at this index
    {
        s->data = value;
        s->size = size;
    }
    else if (!hash_find(set, value))
    {
        struct hash_slot *new = my_malloc(sizeof(struct hash_slot));
        new->data = value;
        new->size = size;
        new->next = NULL;
        while (s->next != NULL)
            s = s->next;
        s->next = new;
    }
}

static int handle_start(struct hash_slot *s)
{
    s->data = s->next->data;
    struct hash_slot *next = s->next;
    s->next = s->next->next;
    next->data = NULL;
    next->next = NULL;
    next->size = 0;
    my_free(next);
    return 1;
}

int hash_remove(struct hash_map *set, void *value)
{
    assert(value != NULL);
    struct hash_slot *s = get_slot(set, value);
    if (s->data == NULL)
        return 0;
    if (s->next == NULL && s->data == value)
    {
        s->data = NULL;
        s->size = 0;
        return 1;
    }
    struct hash_slot *f = s;
    while (s->data != value)
    {
        f = s;
        s = s->next;
        if (s == NULL)
            return 0;
    }
    if (s == f)
        return handle_start(s);
    f->next = s->next;
    s->size = 0;
    s->next = NULL;
    s->data = NULL;
    my_free(s);
    return 1;
}

size_t hash_find(struct hash_map *set, void *value)
{
    assert(value != NULL);
    struct hash_slot *s = get_slot(set, value);
    if (s->data == NULL)
        return 0;
    while (s->data != value)
    {
        s = s->next;
        if (s == NULL)
            return 0;
    }

    return s->size;
}

struct hash_slot* hash_get_slot(struct hash_map *set, void *value)
{
    assert(value != NULL);
    struct hash_slot *s = get_slot(set, value);
    if (s->data == NULL)
        return NULL;
    while (s->data != value)
    {
        s = s->next;
        if (s == NULL)
            return NULL;
    }

    return s;
}

void hash_init(struct hash_map *s, size_t size)
{
    assert(size > 0);
    s->size = size;
    for (size_t i = 0; i < size; ++i)
    {
        s->slots[i].data = NULL;
        s->slots[i].next = NULL;
        s->slots[i].size = 0;
    }
}