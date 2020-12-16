.extern _puts
.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
_myBool$ = -1
	subl    $4, %esp
	movl    $1, %eax
	movl    %eax, _myBool$(%ebp)
..Jump.0:
	movl    _myBool$(%ebp), %eax
	cmpl    $0, %eax
	je      ..Jump.1
	movl    $..String.0, %eax
	push    %eax
	call    _puts
	addl    $4, %esp
	jmp     ..Jump.1
	jmp     ..Jump.0
..Jump.1:
..Jump.2:
	movl    $1, %eax
	movl    %eax, %edx
	movl    _myBool$(%ebp), %eax
	cmpl    %edx, %eax
	movl    $0, %eax
	je      ..Jump.4
	movl    $1, %eax
..Jump.4:
	cmpl    $0, %eax
	je      ..Jump.3
	movl    $..String.1, %eax
	push    %eax
	call    _puts
	addl    $4, %esp
	jmp     ..Jump.4
	jmp     ..Jump.2
..Jump.3:
	movl    %ebp, %esp
	pop     %ebp
	ret
..String.0:
	.string "True"
..String.1:
	.string "False"
