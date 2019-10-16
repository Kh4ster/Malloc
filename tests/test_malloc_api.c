#include <criterion/criterion.h>
#include <string.h>

#include "../src/malloc_api.h"

Test(malloc, basic_small_single_allocation)
{
    int *a = my_malloc(sizeof(int));
    *a = 3;
    cr_assert_eq(3, *a, "incorrect int value");
}

Test(malloc, basic_medium_single_allocation)
{
    unsigned long long *a = my_malloc(sizeof(unsigned long long) * 3);
    a[0] = 123456789;
    a[1] = 456789;
    a[2] = 987654321;
    cr_assert_eq(a[0], 123456789, "incorrect int value");
    cr_assert_eq(a[1], 456789, "incorrect int value");
    cr_assert_eq(a[2], 987654321, "incorrect int value");
    a[0] = 0;
    a[1] = 0;
    a[2] = 0;
    cr_assert_eq(a[0], 0, "incorrect int value");
    cr_assert_eq(a[1], 0, "incorrect int value");
    cr_assert_eq(a[2], 0, "incorrect int value");
}

Test(malloc, basic_large_single_allocation)
{
   int *arr = my_malloc(sizeof(int) * 4000);
   for (size_t i = 0; i < 4000; i++)
        arr[i] = i;
    for (size_t i = 0; i < 4000; i++)
        cr_assert_eq(arr[i], i, "incorrect int value");
}


Test(malloc, basic_small_multiple_allocation)
{
    int *a = my_malloc(sizeof(int));
    *a = 5;
    char *str = my_malloc(sizeof(char) * 20);
    strcpy(str, "Hello world !");
    cr_assert_eq(strcmp(str, "Hello world !"), 0, "invalid string value");
    cr_assert_eq(*a, 5, "incorrect int value");
}

Test(malloc, lotof_small_allocation_single_pagesize)
{
    int **arr = my_malloc(sizeof(int*) * 10);
    for (size_t i = 0; i < 10; i++)
        arr[i] = my_malloc(sizeof(int) * (i + 1));

    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j <= i; j++)
            arr[i][j] = i + j;
    }
    for (size_t i = 0; i < 10; i++)
    {
        for (size_t j = 0; j <= i; j++)
            cr_assert_eq(i + j, arr[i][j]);
    }
}

Test(malloc, lotof_small_allocation_larger_than_pagesize)
{
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
}

Test(malloc, error_malloc_null)
{
    int *a = my_malloc(0);
    cr_assert_null(a);
}

Test(free, null_ptr)
{
    free(NULL);
}