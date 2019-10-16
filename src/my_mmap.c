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