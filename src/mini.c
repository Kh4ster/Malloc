#define _GNU_SOURCE
#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

void *malloc(size_t);
void *realloc(void*, size_t);

int main()
{
    char *a = malloc(3000);
    a[2500] = 5;
    char *b = realloc(a, 5000);
}
