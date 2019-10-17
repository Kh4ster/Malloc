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
    int *a = my_malloc(sizeof(int));
    *a = 3;
    assert(3 == *a && "incorrect int value");
    /*
    int **arr = my_calloc(5000, sizeof(int*));
    for (size_t i = 0; i < 5000; i++)
        arr[i] = my_calloc(5000, sizeof(int));

    for (size_t i = 0; i < 5000; i++)
    {
        arr[i][27] = i;
    }
    for (int i = 0; i < 5000; i++)
    {
        assert(i == arr[i][27]);
    }
    for (size_t i = 0; i < 5000; i++)
    {
        my_free(arr[i]);
    }
    my_free(arr);*/
}
