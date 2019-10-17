#include <criterion/criterion.h>

#include "../src/util.h"
#include "../src/my_mmap.h"
#include "../src/small_allocator.h"

Test(log, correct_value)
{
    for (size_t i = 0; i <= 16; i++)
        cr_assert_eq(0, my_log(i));
    for (size_t i = 17; i <= 32; i++)
        cr_assert_eq(1, my_log(i));
}

Test(get_page_address, correct_page_addr)
{
    init_small_allocator();
    void *ptr = my_mmap();
    char *b = ptr;
    char *a = b + 452;
    cr_assert_eq(ptr, get_page_address(a), "not same address");
}