CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -std=c99

.PHONY = all check mini

all:
	$(CC) $(CFLAGS) $(CPPFLAGS) my_mmap.c small_allocator.c util.c -c

check:
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_util.c util.c my_mmap.c small_allocator.c hash_map.c malloc_api.c -o test_util
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_small_allocator.c util.c my_mmap.c small_allocator.c hash_map.c malloc_api.c -o test_allocator
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_malloc_api.c small_allocator.c util.c my_mmap.c malloc_api.c hash_map.c -o test_api
	./test_util
	./test_allocator
	./test_api
	rm test*

mini:
	$(CC) $(CPPFLAGS) util.c my_mmap.c small_allocator.c malloc_api.c hash_map.c -g
