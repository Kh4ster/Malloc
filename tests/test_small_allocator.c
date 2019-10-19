#include <criterion/criterion.h>

#include "../src/small_allocator.h"

#define NB_GROUP_PAGE 7
#define FIRST_PAGE_SIZE 16

Test(small_allocator, init_freelist)
{
    init_small_allocator();
    size_t size = FIRST_PAGE_SIZE;
    for (size_t i = 0; i < NB_GROUP_PAGE; i++)
    {
        init_block_start(&g_small_allocator, size, &g_small_allocator.heads[i]);
        struct block *head = g_small_allocator.heads[i];
        cr_assert_not_null(head, "head null");
        cr_assert_eq(&g_small_allocator, head->prev, "prev not equ to small allocator");
        cr_assert_null(head->next, "head next not null");
        cr_assert_eq(size, head->sub_block_size, "incorrect size");
        size <<= 1;
    }
    cr_assert_eq(2048, size, "power two calculus failed");
}