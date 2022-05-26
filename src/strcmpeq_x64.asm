global strcmpeq_x64:function

strcmpeq_x64:

	xor	rax, rax
	xor	rdx, rdx

.loop:	mov	cl, [rsi + rdx]	; fetch bytes
	mov	ch, [rdi + rdx]

	test	cx, cx		; if both bytes are null,
	jz	.eql		; strings are equal

	cmp	cl, ch		; compare bytes,
	jne	.dif		; quit if not equal

	inc	rdx		; else go for next round
	jmp	.loop

.eql:	inc	eax
.dif:	ret
