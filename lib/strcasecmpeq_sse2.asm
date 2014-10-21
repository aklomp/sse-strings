extern _GLOBAL_OFFSET_TABLE_
global aldiff:data 16
global uppera:data 16
global upperz:data 16
global strcasecmpeq_sse2:function

align 16
aldiff:	times 16 db 'a' - 'A'	; difference between lower and uppercase
uppera:	times 16 db 'A' - 1	; we test for "greater than"; off by one
upperz:	times 16 db 'Z'

%macro lcase 1
	movdqa	  xmm6, %1	; xmm6 := xxx
	movdqa	  xmm7, %1	; xmm7 := xxx
	pcmpgtb	  xmm6, xmm3	; xmm6 := (xxx >= 'A')
	pcmpgtb	  xmm7, xmm4	; xmm7 := (xxx > 'Z')
	pandn	  xmm7, xmm6	; xmm7 := (xxx >= 'A' && !(xxx > 'Z'))
	pand	  xmm7, xmm5	; xmm7 &= aldiff
	paddusb   %1, xmm7	; xxx += xmm7
%endmacro

strcasecmpeq_sse2:

	xor	rdx, rdx
	pxor	xmm0, xmm0

	movdqa	xmm3, [rel uppera wrt ..got]
	movdqa	xmm4, [rel upperz wrt ..got]
	movdqa	xmm5, [rel aldiff wrt ..got]

.loop:	movdqu	xmm1, [rdi + rdx]	; load strings
	movdqu	xmm2, [rsi + rdx]

	movdqa	xmm6, xmm1	; copy strings
	movdqa	xmm7, xmm2

	pcmpeqb	xmm6, xmm0	; check copies for 0x00
	pcmpeqb	xmm7, xmm0

	pmovmskb  eax, xmm6	; fetch bitmasks
	pmovmskb  ecx, xmm7

	; If both masks are zero, neither strings contain a null byte and we
	; take a fast track where we just check the vectors for equality:

	test	ax, cx
	jnz	.tail

	lcase	xmm1		; lowercase the strings
	lcase	xmm2

	pcmpeqb   xmm1, xmm2	; compare strings directly
	pmovmskb  eax, xmm1	; fetch bitmask

	not	ax		; if the inverse of the bitmask is not zero,
	test	ax, ax		; the strings differ in at least one place
	jnz	.dif

	add	rdx, 16		; match so far; go in for the next loop round
	jmp	.loop

.tail:	; If one mask is zero, one string contains a null byte but the other
	; doesn't; strings are unequal. Must test for this case because bsf is
	; not defined when run on a zero input:

	test	ax, ax
	jz	.dif

	test	cx, cx
	jz	.dif

	; Neither mask is zero, so both vectors contain a null byte somewhere;
	; take a slow path where we compensate for the null bytes:

	bsf	ax, ax		; find bit positions of first null bytes
	bsf	cx, cx

	cmp	ax, cx		; if the nulls do not start at the same byte,
	jne	.dif		; the strings have different lengths; exit

	lcase	xmm1		; lowercase the strings
	lcase	xmm2

	pcmpeqb   xmm1, xmm2	; compare strings
	pmovmskb  eax, xmm1	; fetch bitmask

	; If the inverse of the bitmask is zero, full match.
	; Must again do this check because bsf is not defined for zero input:

	not	ax		; does not affect the flags
	test	ax, ax		; test for zero
	jz	.eql

	bsf	ax, ax		; get first nonmatching position
	cmp	ax, cx		; if this is less than where the nulls start,
	jl	.dif		; the strings differ

	; The strings are equal up to the null byte; fallthrough:

.eql:	mov	ax, 1		; upper bits are known to be zero
	ret

.dif:	xor	rax, rax
	ret
