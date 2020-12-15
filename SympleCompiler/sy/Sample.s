.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
_someString$ = -4
	subl    $4, %esp
	movl    $..String.0, %eax
	movl    %eax, _someString$(%ebp)
	movl    _someString$(%ebp), %eax
	push    %eax
	call    _puts
	addl    $4, %esp
	movl    %ebp, %esp
	pop     %ebp
	ret
..String.0:
	.string "Hello, world!\n"
