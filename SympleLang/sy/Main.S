.global _add
_add: # int add(int, int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_a$0 = 8
_b$0 = 12
	mov     _b$0(%ebp), %eax
	push    %eax
	mov     _a$0(%ebp), %eax
	pop     %ebx
	add     %ebx, %eax
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
	mov     $69, %eax
	push    %eax
	call    _add
	add     $8, %esp
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
