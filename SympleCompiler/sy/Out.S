.global _main
_main:
	push %ebp
	mov %esp, %ebp

	mov $69, %eax

	mov %ebp, %esp
	pop %ebp
	ret
