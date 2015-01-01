#include <stdbool.h>
#include <stdio.h>

#include "sse_level.h"
#include "../include/strlen_sse2.h"
#include "../include/strlen_sse4.h"

static int
test_strlen (const char *name, size_t (*mystrlen)(const char *s))
{
	int got;
	char buf[50];

	// Test strings of various known length:
	for (int i = 0; i < 50; i++) {
		for (int j = 0; j < i; j++) {
			buf[j] = 'A';
		}
		buf[i] = '\0';

		if ((got = mystrlen(buf)) != i) {
			printf("FAIL: %s: '%s': expected %d, got %d\n", name, buf, i, got);
			return 1;
		}
	}
	return 0;
}

int
main ()
{
	int ret = 0;

	if (have_sse2()) {
		ret |= test_strlen("strlen_sse2", strlen_sse2);
	}
	else puts("WARN: not testing SSE2 routines");

	if (have_sse42()) {
		ret |= test_strlen("strlen_sse4", strlen_sse4);
	}
	else puts("WARN: not testing SSE4.2 routines");

	return ret;
}
