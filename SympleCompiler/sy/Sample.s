main:
	movl $5, %eax
	movl %eax, %edx
	movl $5, %eax
	imull %edx, %eax
	ret
@void_notMain:
	movl $3, %eax
	movl %eax, %edx
	movl $2, %eax
	imull %edx, %eax
	movl %eax, %edx
	movl $1, %eax
	addl %edx, %eax
	ret
