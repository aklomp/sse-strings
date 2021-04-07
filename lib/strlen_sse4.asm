global strlen_sse4:function

strlen_sse4:

	xor        eax, eax
	pxor       xmm0, xmm0

.loop:	pcmpistri  xmm0, [rdi + rax], 0x08	; EQUAL_EACH
	lea        rax, [rax + 16]		; inc offset without touching flags
	jnz        .loop			; branch based on pcmpistri's flags

	lea        rax, [rax + rcx - 16]	; subtract spurious final increment
	ret
