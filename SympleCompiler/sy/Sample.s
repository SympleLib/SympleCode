# Function Declaration
main:
# 	Push Stack
	push    %ebp
	movl    %esp, %ebp
	xorl    %eax, %eax
# 	Function Arguments
_c$ = -4
	subl    $4, %esp
_other$ = -8
	subl    $4, %esp
_test$ = -12
	subl    $4, %esp
# Function Call
# 	Push Arguments
	movl    $5, %eax
	push    %eax
	movl    $5, %eax
	push    %eax
# 	Call Function
	call    multiply
# 	Pop Arguments
	addl    $8, %esp
	movl    %eax, _test$(%ebp)
# Binary Expression [=]
	movl    _test$(%ebp), %eax
	movl    %eax, _other$(%ebp)
# Binary Expression [=]
	movl    _other$(%ebp), %eax
	movl    %eax, _c$(%ebp)
	movl    _c$(%ebp), %eax
# 	Pop Stack and Return
	movl    %ebp, %esp
	pop     %ebp
	ret
# Function Declaration
multiply:
# 	Push Stack
	push    %ebp
	movl    %esp, %ebp
	xorl    %eax, %eax
# 	Function Arguments
_a$ = 8
_b$ = 12
# Binary Expression [*]
	movl    _b$(%ebp), %eax
	movl    %eax, %edx
	movl    _a$(%ebp), %eax
	imul    %edx, %eax
# 	Pop Stack and Return
	movl    %ebp, %esp
	pop     %ebp
	ret
