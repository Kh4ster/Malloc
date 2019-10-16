#include <stdint.h>
#include <stdio.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    size_t a;
    if (__builtin_umull_overflow(nmemb, size, &a))
        return NULL;
    else
    {
        return ptr + nmemb * size;
    }
}