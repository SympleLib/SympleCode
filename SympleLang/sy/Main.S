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
.global _main
_main: # int main()
	# Begin Stack Frame
	push    %ebp
	mov     %esp, %ebp
	sub     _main.StackSize, %esp
	lea     _PrintEZ, %eax
	push    %eax
	call    _atexit
	add     $4, %esp
	mov     $69, %eax
	push    %eax
	call    _ExitProcess@4
	add     $4, %esp
	# End Stack Frame
	mov     %ebp, %esp
	pop     %ebp
	ret
.data
_PrintEZ.StackSize:
	.long 0
_main.StackSize:
	.long 0
.bss
