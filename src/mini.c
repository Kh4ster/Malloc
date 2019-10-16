#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "malloc_api.h"
#include "my_mmap.h"

int main()
{
    char *str = my_malloc(sizeof(char) * 20);
    strcpy(str, "Hello world !");
    str = my_realloc(str, 25);
    strcpy(str, "Hello world ! More more");
}