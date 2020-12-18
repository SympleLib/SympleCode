.extern _calloc
.extern _memset
.global _Vector__New
_Vector__New:
	push    %ebp
	movl    %esp, %ebp
_rvec$ = -4
	subl    $4, %esp


	movl    $8, %eax
	pushl   %eax
	call    _calloc
	addl    $4, %esp
	movl    %eax, _rvec$(%ebp)


	movl    $8, %eax
	pushl   %eax

	movl    _rvec$(%ebp), %eax
	popl    %edx
	addl    %edx, %eax
	movl    %ebp, %esp
	pop     %ebp
	ret
.global _Vector__GetSize
_Vector__GetSize:
	push    %ebp
	movl    %esp, %ebp
_vector$ = 8
_rvec$ = -4
	subl    $4, %esp


	movl    $8, %eax
	pushl   %eax

	movl    _vector$(%ebp), %eax
	popl    %edx
	subl    %edx, %eax
	movl    %eax, _rvec$(%ebp)


	movl    _rvec$(%ebp), %eax
	movl    (%eax), %eax
	movl    %ebp, %esp
	pop     %ebp
	ret
.global _Vector__SetSize
_Vector__SetSize:
	push    %ebp
	movl    %esp, %ebp
_vector$ = 8
_size$ = 12
	movl    %ebp, %esp
	pop     %ebp
	ret
.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
	xorl    %eax, %eax
_vec$ = -4
	subl    $4, %esp

	call    _Vector__New
	addl    $4, %esp
	movl    %eax, _vec$(%ebp)


	movl    $1337, %eax
	pushl   %eax

	movl    _vec$(%ebp), %eax
	pushl   %eax
	call    _Vector__SetSize
	addl    $4, %esp


	movl    _vec$(%ebp), %eax
	pushl   %eax
	call    _Vector__GetSize
	addl    $4, %esp
	movl    %ebp, %esp
	pop     %ebp
	ret
