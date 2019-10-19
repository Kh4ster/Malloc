#define _GNU_SOURCE
#include <sys/mman.h>
#include <stddef.h>
#include <unistd.h>

#include "small_allocator.h"
#include "my_mmap.h"

void* my_mmap(void)
{
    static const int prot = PROT_READ | PROT_WRITE;
    static const int flags = MAP_PRIVATE | MAP_ANONYMOUS;
    void *ptr = mmap(NULL, g_small_allocator.page_size, prot, flags, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;
    return ptr;
}

void* my_mmap_size(size_t size)
{
    static const int prot = PROT_READ | PROT_WRITE;
    void *ptr = mmap(NULL, size, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
        return NULL;
    return ptr;
}

void my_munmap(void *ptr, size_t size)
{
    munmap(ptr, size);
}
