#include <criterion/criterion.h>
#include <stdio.h>

#include "../src/small_allocator.h"
#include "../src/malloc_api.h"

#define NB_GROUP_PAGE 7

Test(small_allocator, init_freelist)
{
    init_small_allocator();
    size_t size = 16;
    for (size_t i = 0; i < NB_GROUP_PAGE; i++)
    {
        struct block *head = small_allocator.heads[i];
        cr_assert_not_null(head, "head null");
        cr_assert_eq(&small_allocator, head->prev, "prev not equ to small allocator");
        cr_assert_null(head->next, "head next not null");
        cr_assert_eq(size, head->sub_block_size, "incorrect size");
        size <<= 1;
    }
    cr_assert_eq(2048, size, "power two calculus failed");
}

Test(small_allocator, basic_small_single_allocation)
{
    int *a = malloc(sizeof(int));
    *a = 3;
    cr_assert_eq(3, *a, "incorrect int value");
}

Test(small_allocator, basic_medium_single_allocation)
{
    unsigned long long *a = malloc(sizeof(unsigned long long) * 3);
    a[0] = 123456789;
    a[1] = 456789;
    a[2] = 987654321;
    cr_assert_eq(a[0], 123456789, "incorrect int value");
    cr_assert_eq(a[1], 456789, "incorrect int value");
    cr_assert_eq(a[2], 987654321, "incorrect int value");
}