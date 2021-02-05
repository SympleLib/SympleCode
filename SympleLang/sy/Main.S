.global _add
_add: # int add(int, int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
	mov     $1, %eax
	jmp     _add.Return
_add.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
.global _main
_main: # int main()
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
	mov     $1, %eax
	push    %eax
	mov     $1, %eax
	push    %eax
	call    _add
	add     $8, %esp
	jmp     _main.Return
	mov     $5, %eax
	jmp     _main.Return
_main.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
