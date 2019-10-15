#include <sys/mman.h>
#include <stdio.h>
#include <assert.h>

int main()
{
    void *ptr = mmap(NULL, 4096, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    char *a = (char*)ptr + 500;
    void *addr = (void*)((unsigned long long)a & 0xFFFFFFFFFFFFF000);
    assert(addr == ptr);
}