CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -std=c99

all:
	$(CC) $(CFLAGS) $(CPPFLAGS) my_mmap.c small_allocator.c util.c -c

check:
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_util.c util.c -lm -o test1
	$(CC) $(CFLAGS) $(CPPFLAGS) -lcriterion ../tests/test_small_allocator.c util.c my_mmap.c small_allocator.c -lm -o test2
	./test1
	./test2

mini:
	$(CC) $(CPPFLAGS) my_mmap.c small_allocator.c util.c mini.c -lm