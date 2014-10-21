global strlen_sse4:function

strlen_sse4:

	mov        rax, -16
	pxor       xmm0, xmm0

.loop:	; Must perform addition before the string comparison;
	; doing it after would interfere with the status flags:

	add        rax, 16
	pcmpistri  xmm0, [rdi + rax], 0x08	; EQUAL_EACH
	jnz        .loop

	add        rax, rcx
	ret
