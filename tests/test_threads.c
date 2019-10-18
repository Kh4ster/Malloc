#include <pthread.h>
#include <criterion/criterion.h>
#include <stdio.h>

#include "../src/malloc_api.h"

#define NB_THREADS 277

void *classic(void *args)
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
            cr_assert_eq(i + j, arr[i][j]);
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
            cr_assert_eq(i + j, arr[i][j]);
    }

    for (size_t i = 0; i < 100; i++)
        my_free(arr[i]);
    my_free(arr);

    return NULL;
}

Test(thread, malloc_free)
{
    pthread_t *thread_group = my_malloc(sizeof(pthread_t) * NB_THREADS);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_create(&thread_group[i], NULL, classic, NULL);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_join(thread_group[i], NULL);
}
/*
void *alot_of_small(void *args)
{
    char *a = args;
        a++;

    char **arr = my_malloc(sizeof(char*) * 1000);
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

Test(thread, alot_of_small_allocation)
{
    pthread_t *thread_group = my_malloc(sizeof(pthread_t) * NB_THREADS);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_create(&thread_group[i], NULL, alot_of_small, NULL);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_join(thread_group[i], NULL);
}

int main()
{
    pthread_t *thread_group = my_malloc(sizeof(pthread_t) * NB_THREADS);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_create(&thread_group[i], NULL, alot_of_small, NULL);

    for (size_t i = 0; i < NB_THREADS; ++i)
        pthread_join(thread_group[i], NULL);
}*/