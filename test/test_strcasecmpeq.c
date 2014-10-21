#include <stdio.h>
#include <string.h>
#include <strings.h>

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
	for (int i = 0; i < 50; i++) {
		for (int k = 0; k < i; k++) {
			b1[k] = 'A' + i;
		}
		b1[i] = '\0';
		for (int j = 0; j < 50; j++) {
			for (int k = 0; k < j; k++) {
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

int
main ()
{
	int ret = 0;

	if (__builtin_cpu_supports("sse2")) {
		ret |= test_strcasecmpeq("strcasecmpeq_sse2", strcasecmpeq_sse2);
	}
	return ret;
}
