#ifndef MALLOC_API_H
#define MALLOC_API_H

#include <stddef.h>

void *my_malloc(size_t size);
void free(void *ptr);
void *calloc(size_t number, size_t size);
void *realloc(void *ptr, size_t size);

#endif /* !MALLOC_API_H */