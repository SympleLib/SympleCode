.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
	movl    $_String.0, %eax
	push    %eax
	call    _puts
	addl    $4, %esp
	movl    %ebp, %esp
	pop     %ebp
	ret
_String.0:
	.string "hi!"