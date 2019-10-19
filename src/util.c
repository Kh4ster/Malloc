#include <assert.h>
#include <pthread.h>

#include "util.h"
#include "small_allocator.h"

#define LAST_THREE_BYTE_NULL 0xFFFFFFFFFFFFF000

int my_log(int n)
{
    assert(n >= 0);
    if (n <= 16)
        return 0;
    else if (n <= 32)
        return 1;
    else if (n <= 64)
        return 2;
    else if (n <= 128)
        return 3;
    else if (n <= 256)
        return 4;
    else if (n <= 512)
        return 5;
    else if (n <= 1024)
        return 6;
    assert(0);
}

//Page address when < 4K is just first bytes with 12 last bytes 0
void* get_page_address(void *ptr)
{
    return (void*)((unsigned long long)ptr & LAST_THREE_BYTE_NULL);
}

void my_lock(void)
{
    pthread_mutex_lock(&g_small_allocator.mutex);
}

void my_unlock(void)
{
    pthread_mutex_unlock(&g_small_allocator.mutex);
}
