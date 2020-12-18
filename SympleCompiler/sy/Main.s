.extern _calloc
.extern _memset
.extern _printf
.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
	xorl    %eax, %eax
_vec$ = -4
	subl    $4, %esp

	movl    %eax, _vec$(%ebp)


	movl    %ebp, %esp
	pop     %ebp
	ret
