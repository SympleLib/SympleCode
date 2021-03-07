.global main
main:
	push %ebp
	mov %esp, %ebp
# Function body...
	mov %ebp, %esp
	pop %ebp
	ret
.global add
add:
	push %ebp
	mov %esp, %ebp
# Function body...
	mov %ebp, %esp
	pop %ebp
	ret
