main:
	push   %ebp
	movl   %esp, %ebp
	xorl   %eax, %eax
_c$ = -4
	subl   $4, %esp
	movl   $5, %eax
	push   %eax
	movl   $5, %eax
	push   %eax
	call   multiply
	addl   $8, %esp
	movl   %eax, _c$(%ebp)
	movl   _c$(%ebp), %eax
	movl   %ebp, %esp
	pop    %ebp
	ret
multiply:
	push   %ebp
	movl   %esp, %ebp
	xorl   %eax, %eax
_a$ = 4
_b$ = 8
	movl   _b$(%ebp), %eax
	movl   %eax, %edx
	movl   _a$(%ebp), %eax
	imul   %edx, %eax
	movl   %ebp, %esp
	pop    %ebp
	ret
