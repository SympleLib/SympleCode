.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
	movl    $3, %eax
	push    %eax
	movl    $3, %eax
	push    %eax
	call    _Pow
	addl    $4, %esp
	movl    %ebp, %esp
	pop     %ebp
	ret
