.extern _calloc
.extern _memset
.extern _printf
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
_sz$ = -4
	subl    $4, %esp

	leal    _size$(%ebp), %eax
	movl    %eax, _sz$(%ebp)
_rvec$ = -8
	subl    $4, %esp


	movl    $8, %eax
	pushl   %eax

	movl    _vector$(%ebp), %eax
	popl    %edx
	subl    %edx, %eax
	movl    %eax, _rvec$(%ebp)



	movl    $0, %eax
	movl    %eax, %edx
	movl    _sz$(%ebp), %eax
	addl    %edx, %eax
	movl    (%eax), %eax
	pushl    %eax

	movl    $0, %eax
	pushl   %eax
	movl    _rvec$(%ebp), %eax
	popl    %edx
	addl    %edx, %eax
	popl    %edx
	movb    %dl, (%eax)
	movb    %dl, %al
	movsbl  %al, %eax



	movl    $1, %eax
	movl    %eax, %edx
	movl    _sz$(%ebp), %eax
	addl    %edx, %eax
	movl    (%eax), %eax
	pushl    %eax

	movl    $1, %eax
	pushl   %eax
	movl    _rvec$(%ebp), %eax
	popl    %edx
	addl    %edx, %eax
	popl    %edx
	movb    %dl, (%eax)
	movb    %dl, %al
	movsbl  %al, %eax



	movl    $2, %eax
	movl    %eax, %edx
	movl    _sz$(%ebp), %eax
	addl    %edx, %eax
	movl    (%eax), %eax
	pushl    %eax

	movl    $2, %eax
	pushl   %eax
	movl    _rvec$(%ebp), %eax
	popl    %edx
	addl    %edx, %eax
	popl    %edx
	movb    %dl, (%eax)
	movb    %dl, %al
	movsbl  %al, %eax



	movl    $3, %eax
	movl    %eax, %edx
	movl    _sz$(%ebp), %eax
	addl    %edx, %eax
	movl    (%eax), %eax
	pushl    %eax

	movl    $3, %eax
	pushl   %eax
	movl    _rvec$(%ebp), %eax
	popl    %edx
	addl    %edx, %eax
	popl    %edx
	movb    %dl, (%eax)
	movb    %dl, %al
	movsbl  %al, %eax
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
	addl    $0, %esp
	movl    %eax, _vec$(%ebp)


	movl    $0x1337, %eax
	pushl   %eax

	movl    _vec$(%ebp), %eax
	pushl   %eax
	call    _Vector__SetSize
	addl    $8, %esp


	movl    _vec$(%ebp), %eax
	pushl   %eax
	call    _Vector__GetSize
	addl    $4, %esp
	movl    %ebp, %esp
	pop     %ebp
	ret
