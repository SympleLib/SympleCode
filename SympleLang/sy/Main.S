.global main
main: # int main()
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
	mov     $0, %eax
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
.global add
add: # int add()
	push    %ebp
	mov     %esp, %ebp
	# Push Stack
	# Pop Stack
	mov     %ebp, %esp
	pop     %ebp
	ret
