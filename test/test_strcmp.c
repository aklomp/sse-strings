#include <stdio.h>
#include <string.h>

#include "../include/strcmp_x64.h"

static int
test_strcmp (const char *name, int (*mystrcmp)(const char *a, const char *b))
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

			if ((exp = strcmp(b1, b2)) == (got = mystrcmp(b1, b2))) {
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

	ret |= test_strcmp("strcmp_x64", strcmp_x64);

	return ret;
}
