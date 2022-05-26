global strlen_sse2:function

strlen_sse2:

	xor       eax, eax		; zero the string offset
	pxor      xmm0, xmm0		; zero the comparison register

.loop:	movdqu    xmm1, [rdi + rax]	; unaligned string read
	pcmpeqb   xmm1, xmm0		; compare string against zeroes
	pmovmskb  ecx, xmm1		; create bitmask from result
	lea       eax, [eax + 16]	; inc offset without touching flags
	test      ecx, ecx		; set flags based on bitmask
	jz        .loop			; no bits set means no zeroes found

	bsf       ecx, ecx		; find position of first set bit
	lea       rax, [rax + rcx - 16]	; 64-bit add position to offset
	ret
