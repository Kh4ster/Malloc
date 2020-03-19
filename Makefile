CC = gcc
CPPFLAGS = -D_DEFAULT_SOURCE
CFLAGS = -Wall -Wextra -Werror -std=c99 -fPIC -fno-builtin
LDFLAGS = -shared
VPATH = src

TARGET_LIB = libmalloc.so
OBJS = malloc.o my_mmap.o small_allocator.o util.o hash_map.o malloc_api.o
CALL_LIB = libtracemalloc.so
CALL_OBJS = call_trace.o

all: $(TARGET_LIB)

check:
	$(CC) $(CPPFLAGS) $(CFLAGS) src/malloc.c src/my_mmap.c src/small_allocator.c src/util.c src/hash_map.c src/malloc_api.c tests/test_small_allocator.c -lpthread -lcriterion -g

$(TARGET_LIB): CFLAGS += -pedantic -fvisibility=hidden
$(TARGET_LIB): LDFLAGS += -lpthread
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

debug: CFLAGS += -g
debug: clean $(TARGET_LIB)

trace: $(CALL_LIB)

$(CALL_LIB): CPPFLAGS = -D_GNU_SOURCE
$(CALL_LIB): CFLAGS += -g
$(CALL_LIB): LDFLAGS += -ldl
$(CALL_LIB): $(CALL_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(TARGET_LIB) $(CALL_LIB) $(OBJS) $(CALL_OBJS)

.PHONY: all $(TARGET_LIB) $(CALL_LIB) trace clean
