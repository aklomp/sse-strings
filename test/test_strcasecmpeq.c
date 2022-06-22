#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "sse_level.h"
#include "../include/strcasecmpeq_sse2.h"

static int
strcasecmpeq_ref (const char *a, const char *b)
{
	// Our reference function:
	return (strcasecmp(a, b) == 0);
}

static int
test_strcasecmpeq (const char *name, int (*mystrcasecmpeq)(const char *a, const char *b))
{
	int exp, got;
	char b1[50], b2[50];

	// Generate 50 * 50 strings:
	for (size_t i = 0; i < sizeof (b1); i++) {
		for (size_t k = 0; k < i; k++) {
			b1[k] = 'A' + i;
		}
		b1[i] = '\0';
		for (size_t j = 0; j < sizeof (b2); j++) {
			for (size_t k = 0; k < j; k++) {
				b2[k] = 'a' + j;
			}
			b2[j] = '\0';

			if ((exp = strcasecmpeq_ref(b1, b2)) == (got = mystrcasecmpeq(b1, b2))) {
				continue;
			}
			printf("FAIL: %s: '%s' '%s': expected %d, got %d\n", name, b1, b2, exp, got);
			return 1;
		}
	}
	return 0;
}

static int
test_sse2()
{
	if (have_sse2()) {
		return test_strcasecmpeq("strcasecmpeq_sse2", strcasecmpeq_sse2);
	}
	puts("WARN: not testing SSE2 routines");
	return 0;
}

int
main ()
{
	int ret = 0;

	ret |= test_sse2();

	return ret;
}
