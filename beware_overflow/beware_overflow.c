#include <stdint.h>
#include <stdio.h>

void *beware_overflow(void *ptr, size_t nmemb, size_t size)
{
    char *b = ptr;
    size_t a;
    if (__builtin_umull_overflow(nmemb, size, &a))
        return NULL;
    else
    {
        return b + nmemb * size;
    }
}