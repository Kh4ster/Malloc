CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -pedantic -std=c99

all:
	$(CC) $(CFLAGS) $(CPPFLAGS) my_mmap.c small_allocator.c util.c -c

check:
	gcc -lcriterion -Werror -pedantic -Wextra -Wall -std=c99 ../test/test_util.c util.c -lm -o test
	./test