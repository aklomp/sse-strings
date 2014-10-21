global strlen_sse2:function

strlen_sse2:

	mov       rax, -16		; 64-bit init to -16
	pxor      xmm0, xmm0		; zero the comparison register

.loop:	add       rax, 16		; add 16 to the offset
	movdqu    xmm1, [rdi + rax]	; unaligned string read
	pcmpeqb   xmm1, xmm0		; compare string against zeroes
	pmovmskb  ecx, xmm1		; create bitmask from result
	test      ecx, ecx		; set flags based on bitmask
	jz        .loop			; no bits set means no zeroes found

	bsf       ecx, ecx		; find position of first set bit
	add       rax, rcx		; 64-bit add position to offset
	ret
