#include <math.h>
#include <assert.h>

#include "util.h"

#define LAST_THREE_BYTE_NULL 0xFFFFFFFFFFFFF000

int my_log(int n)
{
    assert(n >= 0);
    //Car notre formule donne des negatifs sinon
    if (n <= 16)
        return 0;
    double d = n;
    n = log2(d - 1.0) - 3;
    return n;
}

//Page address when < 4K is just first bytes with 12 last bytes 0
void* get_page_address(void *ptr)
{
    return (void*)((unsigned long long)ptr & LAST_THREE_BYTE_NULL);
}