#include <unistd.h>
#include "small_allocator.h"
#include "my_mmap.h"

void *malloc(size_t size)
{
    if (size > 0)
    {
        if (size <= sysconf(_SC_PAGESIZE) / 4)
            return insert_small_block(size);
        else
            return my_mmap_size(size);
    }
    return NULL;
}

void free(void *ptr);
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);