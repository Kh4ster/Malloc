#define _GNU_SOURCE
#include <sys/mman.h>
#include <stddef.h>
#include <err.h>
#include <unistd.h>

#include "my_mmap.h"

static inline void memory_end(void)
{
    errx(1, "No memory left");
}

static inline void error_munmap(void)
{
    errx(1, "Error munmap");
}

void* my_mmap(void)
{
    static const int prot = PROT_READ | PROT_WRITE;
    void *ptr = mmap(NULL, sysconf(_SC_PAGESIZE), prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
        memory_end();
    return ptr;
}

void* my_mmap_size(size_t size)
{
    static const int prot = PROT_READ | PROT_WRITE;
    void *ptr = mmap(NULL, size, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED)
        memory_end();
    return ptr;
}

void my_munmap(void *ptr, size_t size)
{
    if (munmap(ptr, size) == -1)
        error_munmap();
}

void *my_mremap(void *ptr, size_t old_size, size_t new_size)
{
    void *new_ptr = mremap(ptr, old_size, new_size, MREMAP_MAYMOVE);
    if (new_ptr == MAP_FAILED)
        memory_end();
    return new_ptr;
}