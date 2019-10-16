#ifndef MY_MEMMAP_H
#define MY_MEMMAP_H

void* my_mmap(void);
void* my_mmap_size(size_t size);
void my_munmap(void *ptr, size_t size);
void *my_mremap(void *ptr, size_t old_size, size_t new_size);

#endif /* !MY_MEMMAP_H */