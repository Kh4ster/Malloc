#include <unistd.h>
#include <stdio.h>

#include "malloc_api.h"

int main()
{
    int *a = malloc(sizeof(int));
    *a = 5;
    printf("%d\n", *a);
}