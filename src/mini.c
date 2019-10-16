#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>

#include "malloc_api.h"
#include "my_mmap.h"

int main()
{
    void *ptr = my_calloc(12345, 123455621);
    assert(ptr == NULL);
}