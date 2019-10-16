#ifndef MALLOC_API_H
#define MALLOC_API_H

#include <stddef.h>

void *my_malloc(size_t size);
void my_free(void *ptr);
void *my_calloc(size_t number, size_t size);
void *my_realloc(void *ptr, size_t size);

#endif /* !MALLOC_API_H */