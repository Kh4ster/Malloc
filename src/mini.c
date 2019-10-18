#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>

#include "malloc_api.h"
#include "hash_map.h"
#include "small_allocator.h"

#define NB_THREADS 30
/*
void *alot_of_small(void *args)
{
    char *a = args;
        a++;

    char *arr[1000];
    for (size_t i = 0; i < 1000; i++)
        arr[i] = my_malloc(sizeof(char) * (i + 1));

    for (size_t i = 0; i < 100; i++)
        my_free(arr[i]);
    my_free(arr);

    arr = my_malloc(sizeof(int*) * 100);
    for (size_t i = 0; i < 100; i++)
        arr[i] = my_malloc(sizeof(int) * (i + 1));

    for (size_t i = 0; i < 100; i++)
    {
        for (size_t j = 0; j <= i; j++)
            arr[i][j] = i + j;
    }
    for (size_t i = 0; i < 100; i++)
    {
        for (size_t j = 0; j <= i; j++)
            cr_assert_eq(i + j, arr[i][j]);
    }

    for (size_t i = 0; i < 100; i++)
        my_free(arr[i]);
    my_free(arr);

    return NULL;
}
*/
int main()
{
/*    pthread_t *thread_group = my_malloc(sizeof(pthread_t) * NB_THREADS);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_create(&thread_group[i], NULL, alot_of_small, NULL);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_join(thread_group[i], NULL);
*/
    char *arr[10000];
    for (size_t i = 0; i < 15; i++)
    {
        for (size_t i = 0; i < 10000; i++)
            arr[i] = malloc(sizeof(char));
    }
}