#include <stdbool.h>
#include <cpuid.h>

bool
have_sse2 (void)
{
	// Return true if SSE2 is supported, false if not.
	unsigned int eax, ebx, ecx, edx = 0;

	(void)__get_cpuid(1, &eax, &ebx, &ecx, &edx);
	return (edx & bit_SSE2);
}

bool
have_sse42 (void)
{
	// Return true if SSE4.2 is supported, false if not.
	unsigned int eax, ebx, ecx = 0, edx;

	(void)__get_cpuid(1, &eax, &ebx, &ecx, &edx);

	// Clang and GCC use different names for this constant
	// (bit_SSE4_2 vs bit_SSE42), so just use the raw value:
	return (ecx & 0x00100000);
}
