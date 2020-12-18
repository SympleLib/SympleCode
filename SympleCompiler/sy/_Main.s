.global _main
_main:
	pushl %ebp
	movl %esp, %ebp
	xorl %eax, %eax
	
_value = -4
	subl $4, %esp
	movl $69, _value(%ebp)
	
	pushl _value(%ebp)
	pushl $_msg
	calll _printf
	addl $8, %esp
	
_ptr = -8
	subl $4, %esp
	leal _value(%ebp), %eax
	movl %eax, _ptr(%ebp)
	
	movl _ptr(%ebp), %eax
	pushl (%eax)
	pushl $_msgPtr
	calll _printf
	addl $8, %esp
	
_ptr2 = -12
	subl $4, %esp
	
	pushl $8
	calll _malloc
	addl $4, %esp
	movl %eax, _ptr2(%ebp)
	
	movl _ptr2(%ebp), %eax
	addl $4, %eax
	movl _ptr(%ebp), %edx
	movl (%edx), %edx
	movl %edx, (%eax)
	
	movl _ptr2(%ebp), %eax
	movl $4, %edx
	addl %edx, %eax
	movl (%eax), %eax
	pushl %eax
	pushl $_msgPtr2
	calll _printf
	addl $8, %esp

	movl %ebp, %esp
	popl %ebp
	ret
_msg:
	.string "value: %i\n"
_msgPtr:
	.string "ptr: %i\n"
_msgPtr2:
	.string "ptr2: %i\n"