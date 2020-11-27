.global main
main: # Declare Function
	# Push Stack
	pushq %rbp
	mov %rsp, %rbp
	movl $2, %eax
	imull $3, %eax
	addl $1, %eax
	movl $6, %eax
	addl $9, %eax
	popq %rbp # Pop Stack
	ret # Exit Function
