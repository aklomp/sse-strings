#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "sse_level.h"
#include "../include/strcmpeq_x64.h"
#include "../include/strcmpeq_sse2.h"
#include "../include/strcmpeq_sse4.h"

static int
strcmpeq_ref (const char *a, const char *b)
{
	// Our reference function:
	return (strcmp(a, b) == 0);
}

static int
test_strcmpeq (const char *name, int (*mystrcmpeq)(const char *a, const char *b))
{
	int exp, got;
	char b1[50], b2[50];

	// Generate 50 * 50 strings:
	for (int i = 0; i < 50; i++) {
		for (int k = 0; k < i; k++) {
			b1[k] = 'A' + i;
		}
		b1[i] = '\0';
		for (int j = 0; j < 50; j++) {
			for (int k = 0; k < j; k++) {
				b2[k] = 'A' + j;
			}
			b2[j] = '\0';

			if ((exp = strcmpeq_ref(b1, b2)) == (got = mystrcmpeq(b1, b2))) {
				continue;
			}
			printf("FAIL: %s: '%s' '%s': expected %d, got %d\n", name, b1, b2, exp, got);
			return 1;
		}
	}
	return 0;
}

static int
test_x64 (void)
{
	return test_strcmpeq("strcmpeq_x64", strcmpeq_x64);
}

static int
test_sse2 (void)
{
	if (have_sse2()) {
		return test_strcmpeq("strcmpeq_sse2", strcmpeq_sse2);
	}
	puts("WARN: not testing SSE2 routines");
	return 0;
}

static int
test_sse4 (void)
{
	if (have_sse42()) {
		return test_strcmpeq("strcmpeq_sse4", strcmpeq_sse4);
	}
	puts("WARN: not testing SSE4.2 routines");
	return 0;
}

int
main ()
{
	int ret = 0;

	ret |= test_x64();
	ret |= test_sse2();
	ret |= test_sse4();

	return ret;
}
