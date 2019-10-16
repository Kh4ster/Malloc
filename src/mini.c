#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#include "malloc_api.h"
#include "my_mmap.h"

int main()
{
    static const int prot = PROT_READ | PROT_WRITE;
    void *small = my_mmap();
    void *medium = mmap(NULL, 5000, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    void *big = mmap(NULL, 9000, prot, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    printf("%lld %lld %lld\n", (long long)small % 50, (long long)medium % 50, (long long)big % 50);
}