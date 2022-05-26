CFLAGS  += -std=c99 -Wall -Wextra -Werror -pedantic

PROGS = \
  test_strlen \
  test_strcmp \
  test_strcmpeq \
  test_strcasecmpeq

STRLEN_OBJS       := test_strlen.o strlen_sse2.o strlen_sse4.o sse_level.o
STRCMP_OBJS       := test_strcmp.o strcmp_x64.o
STRCMPEQ_OBJS     := test_strcmpeq.o strcmpeq_x64.o strcmpeq_sse2.o strcmpeq_sse4.o sse_level.o
STRCASECMPEQ_OBJS := test_strcasecmpeq.o strcasecmpeq_sse2.o sse_level.o

# Default to silent mode, run 'make V=1' for a verbose build.
ifneq ($(V),1)
Q := @
MAKEFLAGS += --no-print-directory
endif

.PHONY: all test clean

# The default target rebuilds all tests and runs them.
test: clean all
	$(Q)for i in $(PROGS); do printf "  TEST  $$i\n"; ./$$i || exit; done

all: $(PROGS)

# Set per-program object file dependencies.
test_strlen: $(STRLEN_OBJS)
test_strcmp: $(STRCMP_OBJS)
test_strcmpeq: $(STRCMPEQ_OBJS)
test_strcasecmpeq: $(STRCASECMPEQ_OBJS)

$(PROGS):
	@printf "  LD    $@\n"
	$(Q)$(CC) -o $@ $^

%.o: %.c
	@printf "  CC    $@\n"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $^

str%.o: ../src/str%.asm
	@printf "  NASM  $@\n"
	$(Q)nasm -f elf64 -o $@ $^

clean:
	@printf "  CLEAN\n"
	$(Q)$(RM) $(STRLEN_OBJS) $(STRCMP_OBJS) $(STRCMPEQ_OBJS) $(STRCASECMPEQ_OBJS) $(PROGS)
