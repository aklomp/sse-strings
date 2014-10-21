global strcmpeq_sse4:function

strcmpeq_sse4:

	xor        rax, rax
	xor        rdx, rdx

.loop:	movdqu     xmm1, [rdi + rdx]
	pcmpistri  xmm1, [rsi + rdx], 0x18	; EQUAL_EACH | NEGATIVE_POLARITY
	jc         .dif
	jz         .eql
	add        rdx, 16
	jmp        .loop

.eql:   inc        eax
.dif:   ret
