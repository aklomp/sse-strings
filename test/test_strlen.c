#include <stdbool.h>
#include <stdio.h>

#include "sse_level.h"
#include "../include/strlen_sse2.h"
#include "../include/strlen_sse4.h"

static int
test_strlen (const char *name, size_t (*mystrlen)(const char *s))
{
	size_t got;
	char buf[50];

	// Test strings of various known length:
	for (size_t i = 0; i < sizeof (buf); i++) {
		for (size_t j = 0; j < i; j++) {
			buf[j] = 'A';
		}
		buf[i] = '\0';

		if ((got = mystrlen(buf)) != i) {
			printf("FAIL: %s: '%s': expected %zu, got %zu\n", name, buf, i, got);
			return 1;
		}
	}
	return 0;
}

static int
test_sse2 (void)
{
	if (have_sse2()) {
		return test_strlen("strlen_sse2", strlen_sse2);
	}
	puts("WARN: not testing SSE2 routines");
	return 0;
}

static int
test_sse4 (void)
{
	if (have_sse42()) {
		return test_strlen("strlen_sse4", strlen_sse4);
	}
	puts("WARN: not testing SSE4.2 routines");
	return 0;
}

int
main ()
{
	int ret = 0;

	ret |= test_sse2();
	ret |= test_sse4();

	return ret;
}
