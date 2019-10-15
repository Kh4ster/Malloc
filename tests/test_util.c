#include <criterion/criterion.h>

#include "../src/util.h"

Test(util, log)
{
    for (size_t i = 0; i <= 16; i++)
        cr_assert_eq(0, my_log(i));
    for (size_t i = 17; i <= 32; i++)
        cr_assert_eq(1, my_log(i));
}