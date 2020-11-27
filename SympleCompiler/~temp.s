.global main
.global print
message$ = 8 # Set stack value of message to 0
main: # Declare Function
	# Push Stack
	pushq %rbp
	mov %rsp, %rbp
	subq $8, %rsp # Allocate 8 bytes to the stack
	movabsq $.message, %rcx
	movq %rcx, -message$(%rbp)
	callq print
	movabsq $.message, %rcx
	movq %rcx, -message$(%rbp)
	callq print
	popq %rbp # Pop Stack
	ret # Exit Function
print:
	leaq .message(%rip), %rcx
	subq $72, %rsp
	movq %r9, 104(%rsp)
	movq	%r8, 96(%rsp)
	movq	%rdx, 88(%rsp)
	movq	%rcx, 64(%rsp)
	leaq	88(%rsp), %rax
	leaq	88(%rsp), %rax
	movq	%rax, 48(%rsp)
	movq	48(%rsp), %r9
	movq	64(%rsp), %rdx
	movl	$1, %ecx
	movq	%r9, 40(%rsp)
	movq	%rdx, 32(%rsp)
	callq __acrt_iob_func
	xorl	%ecx, %ecx
	movl	%ecx, %r8d
	movq	%rax, %rcx
	movq	32(%rsp), %rdx
	movq	40(%rsp), %r9
	callq _vfprintf_l
	retq
.message:
	.asciz "Test 1!"
.message:
	.asciz "Test 2!"
