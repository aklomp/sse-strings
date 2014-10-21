CFLAGS += -std=c99 -Wall -Wextra -Werror
LDFLAGS +=

.PHONY: all clean

all: \
  test_strlen

test_strlen: test_strlen.o strlen_sse2.o
	$(CC) $(LDFLAGS) -o $@ $^

test_%.o: test_%.c
	$(CC) $(CFLAGS) -o $@ -c $^

str%.o: ../lib/str%.asm
	nasm -f elf64 -o $@ $^

clean:
	rm -f *.o \
	  test_strlen
