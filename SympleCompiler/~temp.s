.global main
.global _Dat$1
.global _Dat$2
.global print
message$ = 8 # Set stack value of message to 0
main: # Declare Function
	# Push Stack
	pushq %rbp
	mov  %rsp, %rbp
	subq $8, %rsp # Allocate 8 bytes to the stack
	# Set message to 'Test 1!\n'
	leaq _Dat$1(%rip), %rcx
	movq %rcx, -message$(%rbp)
	call print
	# Set message to 'Test 2!\n'
	leaq _Dat$2(%rip), %rcx
	movq %rcx, -message$(%rbp)
	call print
	popq %rbp # Pop Stack
	ret # Exit Function
print:
	movq -message$(%rbp), %rcx
	subq $72, %rsp
	movq %r9, 104(%rsp)
	movq %r8, 96(%rsp)
	movq %rdx, 88(%rsp)
	movq %rcx, 64(%rsp)
	leaq 88(%rsp), %rax
	leaq 88(%rsp), %rax
	movq %rax, 48(%rsp)
	movq 48(%rsp), %r9
	movq 64(%rsp), %rdx
	movl $1, %ecx
	movq %r9, 40(%rsp)
	movq %rdx, 32(%rsp)
	call __acrt_iob_func
	xorl %ecx, %ecx
	movl %ecx, %r8d
	movq %rax, %rcx
	movq 32(%rsp), %rdx
	movq 40(%rsp), %r9
	call _vfprintf_l
	movl %eax, 60(%rsp)
	movl 60(%rsp), %eax
	addq $72, %rsp
	retq
_Dat$1:
	.string "Test 1!\n"
_Dat$2:
	.string "Test 2!\n"
