#include <unistd.h>
#include <stdio.h>
#include "small_allocator.h"
#include "my_mmap.h"

void *my_malloc(size_t size)
{
    if (small_allocator.heads[0] == NULL)
        init_small_allocator();
    if (size > 0)
    {
        if (size <= small_allocator.max_sub_block_size)
            return allocate_small_block(size);
        else
            return my_mmap_size(size);
    }
    return NULL;
}

void free(void *ptr);
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);