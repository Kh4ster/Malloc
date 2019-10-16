#include <criterion/criterion.h>
#include <stdio.h>

#include "../src/small_allocator.h"

#define NB_GROUP_PAGE 7

Test(small_allocat, init_freelist)
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