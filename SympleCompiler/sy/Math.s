.global _Add
_Add:
	push    %ebp
	movl    %esp, %ebp
_left$ = 8
_right$ = 12
	movl    _right$(%ebp), %eax
	movl    %eax, %edx
	movl    _left$(%ebp), %eax
	addl    %edx, %eax
	movl    %ebp, %esp
	pop     %ebp
	ret
.global _Sub
_Sub:
	push    %ebp
	movl    %esp, %ebp
_left$ = 8
_right$ = 12
	movl    _right$(%ebp), %eax
	movl    %eax, %edx
	movl    _left$(%ebp), %eax
	subl    %edx, %eax
	movl    %ebp, %esp
	pop     %ebp
	ret
.global _Mul
_Mul:
	push    %ebp
	movl    %esp, %ebp
_left$ = 8
_right$ = 12
	movl    _right$(%ebp), %eax
	movl    %eax, %edx
	movl    _left$(%ebp), %eax
	imul    %edx, %eax
	movl    %ebp, %esp
	pop     %ebp
	ret
.global _Div
_Div:
	push    %ebp
	movl    %esp, %ebp
_left$ = 8
_right$ = 12
_numTimes$ = -4
	subl    $4, %esp
	movl    $0, %eax
	movl    %eax, _numTimes$(%ebp)
..Jump.0:
	movl    _left$(%ebp), %eax
	cmpl    $0, %eax
	je ..Jump.1
	movl    _right$(%ebp), %eax
	movl    %eax, %edx
	movl    _left$(%ebp), %eax
	subl    %edx, %eax
	movl    %eax, _left$(%ebp)
	movl    $1, %eax
	movl    %eax, %edx
	movl    _numTimes$(%ebp), %eax
	addl    %edx, %eax
	movl    %eax, _numTimes$(%ebp)
	jmp ..Jump.0
..Jump.1:
	movl    _numTimes$(%ebp), %eax
	movl    %ebp, %esp
	pop     %ebp
	ret
.global _Pow
_Pow:
	push    %ebp
	movl    %esp, %ebp
_left$ = 8
_right$ = 12
_numTimes$ = -4
	subl    $4, %esp
	movl    $0, %eax
	movl    %eax, _numTimes$(%ebp)
_out$ = -8
	subl    $4, %esp
	movl    $1, %eax
	movl    %eax, _out$(%ebp)
..Jump.2:
	movl    _right$(%ebp), %eax
	movl    %eax, %edx
	movl    _numTimes$(%ebp), %eax
	cmpl    %edx, %eax
	movl    $0, %eax
	je      ..Jump.4
	movl    $1, %eax
..Jump.4:
	cmpl    $0, %eax
	je ..Jump.3
	movl    _left$(%ebp), %eax
	movl    %eax, %edx
	movl    _out$(%ebp), %eax
	imul    %edx, %eax
	movl    %eax, _out$(%ebp)
	movl    $1, %eax
	movl    %eax, %edx
	movl    _numTimes$(%ebp), %eax
	addl    %edx, %eax
	movl    %eax, _numTimes$(%ebp)
	jmp ..Jump.2
..Jump.3:
	movl    _out$(%ebp), %eax
	movl    %ebp, %esp
	pop     %ebp
	ret
