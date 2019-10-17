#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "malloc_api.h"
#include "hash_map.h"
#include "small_allocator.h"

int main(void)
{
    char *a = my_malloc(3000);
    a[2500] = 5;
    char *b = my_realloc(a, 5000);
    assert(a != b);
    assert(b[2500] == 5);
    b[3500] = 3;
    struct hash_slot *slot = hash_get_slot(&g_small_allocator.map, a);
    assert(slot == NULL);
    slot = hash_get_slot(&g_small_allocator.map, b);
    assert(slot != NULL);
    assert(slot->size == 5000);
}
