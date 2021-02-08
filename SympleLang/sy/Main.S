.global _main
_main: # int main()
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_EChar$0 = 0
	sub     $4, %esp
	mov     $69, %eax
	mov     %eax, _EChar$0(%ebp)
	mov     _EChar$0(%ebp), %eax
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
