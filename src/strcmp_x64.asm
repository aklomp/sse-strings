global strcmp_x64:function

strcmp_x64:

	xor	eax, eax
	xor	ecx, ecx
	xor	rdx, rdx

.loop:	mov	cl, [rsi + rdx]	; fetch bytes
	mov	al, [rdi + rdx]

	mov	r8b, cl		; move byte to temporary
	or	r8b, al		; if both bytes are null,
	jz	.ret		; strings are equal

	sub	ax, cx		; subtract bytes,
	jne	.ext		; result from -255 to 255

	inc	rdx		; ax was found to be zero,
	jmp	.loop		; go for next round

.ext:	cwde			; sign-extend ax to eax
	cdqe			; sign-extend eax to rax

.ret:	ret
