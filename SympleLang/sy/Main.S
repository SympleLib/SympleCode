.global _pos
_pos: # int pos(int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_a$0 = 8
	mov     _a$0(%ebp), %eax
	jmp     _pos.Return
_pos.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
.global _neg
_neg: # int neg(int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_a$0 = 8
	mov     _a$0(%ebp), %eax
	neg     %eax
	jmp     _neg.Return
_neg.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
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
	pop     %edx
	add     %edx, %eax
	jmp     _add.Return
_add.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
.global _sub
_sub: # int sub(int, int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_a$0 = 8
_b$0 = 12
	mov     _b$0(%ebp), %eax
	push    %eax
	mov     _a$0(%ebp), %eax
	pop     %edx
	sub     %edx, %eax
	jmp     _sub.Return
_sub.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
.global _mul
_mul: # int mul(int, int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_a$0 = 8
_b$0 = 12
	mov     _b$0(%ebp), %eax
	push    %eax
	mov     _a$0(%ebp), %eax
	pop     %edx
	imul    %edx, %eax
	jmp     _mul.Return
_mul.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
.global _div
_div: # int div(int, int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_a$0 = 8
_b$0 = 12
	mov     _b$0(%ebp), %eax
	push    %eax
	mov     _a$0(%ebp), %eax
	pop     %edx
	mov     %edx, %ecx
	cltd
	idiv    %ecx
	jmp     _div.Return
_div.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
.global _mod
_mod: # int mod(int, int)
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
_a$0 = 8
_b$0 = 12
	mov     _b$0(%ebp), %eax
	push    %eax
	mov     _a$0(%ebp), %eax
	pop     %edx
	mov     %edx, %ecx
	cltd
	idiv    %ecx
	mov     %edx, %eax
	jmp     _mod.Return
_mod.Return:
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
	call    _pos
	add     $4, %esp
	mov     $1, %eax
	push    %eax
	call    _neg
	add     $4, %esp
	mov     $1, %eax
	push    %eax
	mov     $1, %eax
	push    %eax
	call    _add
	add     $8, %esp
	mov     $1, %eax
	push    %eax
	mov     $1, %eax
	push    %eax
	call    _sub
	add     $8, %esp
	mov     $1, %eax
	push    %eax
	mov     $1, %eax
	push    %eax
	call    _mul
	add     $8, %esp
	mov     $1, %eax
	push    %eax
	mov     $1, %eax
	push    %eax
	call    _div
	add     $8, %esp
	mov     $1, %eax
	push    %eax
	mov     $1, %eax
	push    %eax
	call    _mod
	add     $8, %esp
	mov     $0, %eax
	jmp     _main.Return
_main.Return:
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
