.global _main
_main: # int main()
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
	mov     @@ERROR, %eax
	mov     $0, %eax
	jmp     _main.Return
_main.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
