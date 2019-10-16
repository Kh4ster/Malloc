#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>

#include "malloc_api.h"
#include "my_mmap.h"

int main()
{
    char *str[300] = {0};
    str[0] = my_malloc(sizeof(char) * 16);

    for(size_t i = 1; i < 300; ++i)
        str[i] = my_malloc(sizeof(char) * 16);

    for(size_t i = 1; i < 300; ++i)
        str[i][0] = '5';

    for(size_t i = 1; i < 300; ++i)
        my_free(str[i]);

    for(size_t i = 1; i < 300; ++i)
        str[i] = my_malloc(sizeof(char) * 16);

    for(size_t i = 1; i < 300; ++i)
        str[i][0] = '5';
}