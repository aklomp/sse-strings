CFLAGS += -std=c99 -Wall -Wextra -Werror
LDFLAGS +=

.PHONY: all test clean

all: \
  test_strlen \
  test_strcmp \
  test_strcmpeq \
  test_strcasecmpeq

test_strlen: test_strlen.o strlen_sse2.o strlen_sse4.o sse_level.o
	$(CC) $(LDFLAGS) -o $@ $^

test_strcmp: test_strcmp.o strcmp_x64.o
	$(CC) $(LDFLAGS) -o $@ $^

test_strcmpeq: test_strcmpeq.o strcmpeq_x64.o strcmpeq_sse2.o strcmpeq_sse4.o sse_level.o
	$(CC) $(LDFLAGS) -o $@ $^

test_strcasecmpeq: test_strcasecmpeq.o strcasecmpeq_sse2.o sse_level.o
	$(CC) $(LDFLAGS) -o $@ $^

test_%.o: test_%.c
	$(CC) $(CFLAGS) -o $@ -c $^

str%.o: ../lib/str%.asm
	nasm -f elf64 -o $@ $^

sse_level.o: sse_level.c
	$(CC) $(CFLAGS) -o $@ -c $^

# This target rebuilds all tests and runs them:
test: clean all
	./test_strlen
	./test_strcmp
	./test_strcmpeq
	./test_strcasecmpeq

clean:
	rm -f *.o \
	  test_strlen \
	  test_strcmp \
	  test_strcmpeq \
	  test_strcasecmpeq
