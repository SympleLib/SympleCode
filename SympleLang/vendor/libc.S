.text
.global _PrintEZ
_PrintEZ: # void PrintEZ()
	# Begin Stack Frame
	push    %ebp
	mov     %esp, %ebp
	sub     _PrintEZ.StackSize, %esp
	mov     $69, %eax
	push    %eax
	call    _putchar
	add     $4, %esp
	mov     $90, %eax
	push    %eax
	call    _putchar
	add     $4, %esp
	# End Stack Frame
	mov     %ebp, %esp
	pop     %ebp
	ret
.data
_PrintEZ.StackSize:
	.long 0
.bss
