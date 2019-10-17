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
    int **arr = my_calloc(800, sizeof(int*));
    for (size_t i = 0; i < 800; i++)
        arr[i] = my_calloc(800, sizeof(int));

    for (size_t i = 0; i < 800; i++)
    {
        arr[i][27] = i;
    }
    for (size_t i = 0; i < 800; i++)
    {
        assert(i == arr[i][27]);
    }
    for (size_t i = 0; i < 800; i++)
    {
        my_free(arr[i]);
    }
    my_free(arr);
}
