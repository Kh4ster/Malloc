#include <stddef.h>
#include <signal.h>
#include <criterion/criterion.h>
#include <string.h>

#include "../src/malloc_api.h"
#include "../src/small_allocator.h"
#include "../src/hash_map.h"

Test(malloc, basic_small_single_allocation)
{
    int *a = my_malloc(sizeof(int));
    *a = 3;
    cr_assert_eq(3, *a, "incorrect int value");
}

Test(malloc, 0)
{
    int *a = my_malloc(0);
    cr_assert_null(a);
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
    my_free(NULL);
}

Test(free, basic_alloc_free)
{
    char *str = my_malloc(sizeof(char) * 20);
    strcpy(str, "Hello world !");
    cr_assert_eq(strcmp(str, "Hello world !"), 0, "invalid string value");
    my_free(str);
    str = my_malloc(sizeof(char) * 20);
    strcpy(str, "Hello world !");
    cr_assert_eq(strcmp(str, "Hello world !"), 0, "invalid string value");
    my_free(str);
}

Test(free, big_block, .signal = SIGSEGV)
{
    int *ptr = my_malloc(5000);
    *ptr = 5;
    cr_assert_not_null(hash_find(&g_small_allocator.map, ptr));
    my_free(ptr);
    *ptr = 5;
}

Test(free, fill_page_then_free_it)
{
    cr_assert_null(g_small_allocator.heads[0]);

    char *str[300];
    str[0] = my_malloc(sizeof(char) * 16);

    cr_assert_not_null(g_small_allocator.heads[0]);
    cr_assert_null(g_small_allocator.heads[0]->next);

    for(size_t i = 1; i < 300; ++i)
        str[i] = my_malloc(sizeof(char) * 16);

    for(size_t i = 1; i < 7; ++i)
        cr_assert_null(g_small_allocator.heads[i]);

    cr_assert_not_null(g_small_allocator.heads[0]->next);
    cr_assert_null(g_small_allocator.heads[0]->beg_freelist);

    for(size_t i = 1; i < 300; ++i)
        str[i][0] = '5';

    for(size_t i = 1; i < 300; ++i)
        my_free(str[i]);

    cr_assert_not_null(g_small_allocator.heads[0]->beg_freelist);

    for(size_t i = 1; i < 300; ++i)
        str[i] = my_malloc(sizeof(char) * 16);

    for(size_t i = 1; i < 300; ++i)
        str[i][0] = '5';

    cr_assert_null(g_small_allocator.heads[0]->beg_freelist);
}

Test(calloc, overflow)
{
    void *ptr = my_calloc(12345678904, 12345678904);
    cr_assert_null(ptr, "not null after overflow");
}

Test(calloc, basic_allocation)
{
    int *ptr = my_calloc(50, sizeof(int));
    cr_assert_not_null(ptr);
    for (size_t i  = 0; i < 50; ++i)
        cr_assert_eq(ptr[i], 0, "not equal to 0");
    for (size_t i  = 0; i < 50; ++i)
        ptr[i] = i;
    for (size_t i  = 0; i < 50; ++i)
        cr_assert_eq(ptr[i], i, "not equal to value");
    my_free(ptr);
    ptr = my_calloc(50, sizeof(int));
    for (size_t i  = 0; i < 50; ++i)
        ptr[i] = i;
    for (size_t i  = 0; i < 50; ++i)
        cr_assert_eq(ptr[i], i, "not equal to value");
}

Test(realloc, null)
{
    int *ptr = my_realloc(NULL, 50 * sizeof(int));
    cr_assert_not_null(ptr);
    for (size_t i  = 0; i < 50; ++i)
        ptr[i] = i;
    for (size_t i  = 0; i < 50; ++i)
        cr_assert_eq(ptr[i], i, "not equal to value");
    my_free(ptr);
    ptr = my_realloc(NULL, 50 * sizeof(int));
    for (size_t i  = 0; i < 50; ++i)
        ptr[i] = i;
    for (size_t i  = 0; i < 50; ++i)
        cr_assert_eq(ptr[i], i, "not equal to value");
}

Test(realloc, stay_in_table)
{
    char *str = my_malloc(sizeof(char) * 20);
    strcpy(str, "Hello world !");
    cr_assert_eq(strcmp(str, "Hello world !"), 0, "invalid string value");
    str = my_realloc(str, 25);
    cr_assert_eq(strcmp(str, "Hello world !"), 0, "invalid string value");
    strcpy(str, "Hello world ! More more");
    cr_assert_eq(strcmp(str, "Hello world ! More more"), 0, "invalid string value");
}
Test(realloc, change_table)
{
    char *str = my_malloc(sizeof(char) * 20);
    strcpy(str, "Hello world !");
    cr_assert_eq(strcmp(str, "Hello world !"), 0, "invalid string value");
    str = my_realloc(str, 50);
    struct freelist_item *first = g_small_allocator.heads[1]->beg_freelist;
    //suppose to free first so first free block should be right after
    cr_assert_eq(first, g_small_allocator.heads[1] + 1);
    cr_assert_eq(strcmp(str, "Hello world !"), 0, "invalid string value");
    strcpy(str, "Hello world ! More more more more more more");
    cr_assert_eq(strcmp(str, "Hello world ! More more more more more more"), 0, "invalid string value");
}

Test(realloc, big_same_addr)
{
    char *a = my_malloc(3000);
    *a = 5;
    char *b = my_realloc(a, 3500);
    b[3250] = 5;
    cr_assert_eq(a, b);
    cr_assert_eq(*b, 5);
    struct hash_slot *slot = hash_get_slot(&g_small_allocator.map, a);
    cr_assert_eq(slot->size, 3500);
}

Test(realloc, big_diff_addr)
{
    char *a = my_malloc(3000);
    a[2500] = 5;
    char *b = my_realloc(a, 5000);
    cr_assert_(a != b, "a should not be equal to b");
    cr_assert_eq(b[2500], 5);
    b[3500] = 3;
    struct hash_slot *slot = hash_get_slot(&g_small_allocator.map, a);
    cr_assert_null(slot);
    slot = hash_get_slot(&g_small_allocator.map, b);
    cr_assert_not_null(slot);
    cr_assert_eq(slot->size, 5000);
}

Test(malloc_free, alot_of_big_block)
{
    int **arr = my_calloc(200, sizeof(int*));
    for (size_t i = 0; i < 200; i++)
        arr[i] = my_calloc(800, sizeof(int));

    for (size_t i = 0; i < 200; i++)
    {
        arr[i][27] = i;
    }
    for (size_t i = 0; i < 200; i++)
    {
        cr_assert_eq(i, arr[i][27]);
    }
}

/*
int main()
{
    int **arr = my_calloc(200, sizeof(int*));
    for (size_t i = 0; i < 200; i++)
        arr[i] = my_calloc(800, sizeof(int));

    for (size_t i = 0; i < 200; i++)
    {
        arr[i][27] = i;
    }
    for (size_t i = 0; i < 200; i++)
    {
        cr_assert_eq(i, arr[i][27]);
    }
}
*/