CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -std=c99

.PHONY = all check mini

all:
	$(CC) $(CFLAGS) $(CPPFLAGS) my_mmap.c small_allocator.c util.c -c

check:
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_util.c util.c my_mmap.c small_allocator.c hash_map.c malloc_api.c -o test_util -pthread
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_small_allocator.c util.c my_mmap.c small_allocator.c hash_map.c malloc_api.c -o test_allocator -pthread
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_malloc_api.c small_allocator.c util.c my_mmap.c malloc_api.c hash_map.c -o test_api -pthread
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_threads.c small_allocator.c util.c my_mmap.c malloc_api.c hash_map.c -o test_threads -pthread
	./test_util
	./test_allocator
	./test_api
	./test_threads
	rm test*

malloc:
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_malloc_api.c small_allocator.c util.c my_mmap.c malloc_api.c hash_map.c -o test_api -g -pthread

mini:
	$(CC) $(CPPFLAGS) mini.c util.c my_mmap.c small_allocator.c malloc_api.c hash_map.c -g -pthread
