.extern _puts
.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
_myBool$ = -1
	subl    $4, %esp
	movl    $0, %eax
	movl    %eax, _myBool$(%ebp)
	movl    _myBool$(%ebp), %eax
	cmpl    $0, %eax
	je      ..Jump.1
	movl    $..String.0, %eax
	push    %eax
	call    _puts
	addl    $4, %esp
	jmp     ..Jump.0
..Jump.1:
	movl    $..String.1, %eax
	push    %eax
	call    _puts
	addl    $4, %esp
..Jump.0:
	movl    %ebp, %esp
	pop     %ebp
	ret
..String.0:
	.string "True"
..String.1:
	.string "False"
