#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#include "malloc_api.h"
#include "my_mmap.h"

int main()
{
    printf("%lu, %lu\n", sizeof(size_t), sizeof(long long));
}