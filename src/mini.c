#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "malloc_api.h"

#define NB_THREADS 1

void *action(void *args)
{
    //for compilation flags
    char *a = args;
    a++;

    int **arr = my_malloc(sizeof(int*) * 100);
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
            assert(i + j == arr[i][j]);
    }

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
            assert(i + j == arr[i][j]);
    }

    for (size_t i = 0; i < 100; i++)
        my_free(arr[i]);
    my_free(arr);

    return NULL;
}

int main(void)
{
    /*pthread_t *thread_group = my_malloc(sizeof(pthread_t) * NB_THREADS);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_create(&thread_group[i], NULL, action, NULL);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_join(thread_group[i], NULL);*/

    action(NULL);
}