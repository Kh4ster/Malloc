#ifndef HASH_SET_H
#define HASH_SET_H

#include <stddef.h>

typedef size_t (*hash_function)(void *key, size_t table_size);

#define NB_SLOTS 256

struct hash_slot
{
    const void *data;
    size_t size;
    struct hash_slot *next;
};

struct hash_map
{
    struct hash_slot slots[NB_SLOTS];
    size_t size;
};

size_t basic_hash_function(long long key, size_t table_size);
void hash_init(struct hash_map *s, size_t size);
void hash_free(struct hash_map *set);
void hash_insert(struct hash_map *set, void *value, size_t size);
int hash_remove(struct hash_map *set, void *value);
size_t hash_find(const struct hash_map *set, void *value);
void hash_dump(const struct hash_map *set);

#endif /* !HASH_SET_H */
