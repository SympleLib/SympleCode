.text
.global _main
_main: # int main()
	# Begin Stack Frame
	push    %ebp
	mov     %esp, %ebp
	sub     _main.StackSize, %esp
_EChar$0 = 0
	mov     $69, %eax
	mov     %eax, _EChar$0(%ebp)
	mov     _EChar$0(%ebp), %eax
	push    %eax
	call    _putchar
	add     $4, %esp
	mov     $0, %eax
	jmp     _main.Return
_main.Return:
	# End Stack Frame
	mov     %ebp, %esp
	pop     %ebp
	ret
.data
_main.StackSize:
	.long 4
.bss
