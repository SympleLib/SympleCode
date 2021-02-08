.global _main
_main: # int main()
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
	mov     $69, %eax
	push    %eax
	call    _putchar
	add     $4, %esp
	mov     $0, %eax
	jmp     _main.Return
_main.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
