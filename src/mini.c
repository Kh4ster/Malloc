#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "malloc_api.h"
#include "my_mmap.h"
#include "hash_map.h"
#include "small_allocator.h"

int main()
{
    char *a = my_malloc(3000);
    a[2500] = 5;
    char *b = my_realloc(a, 5000);
}