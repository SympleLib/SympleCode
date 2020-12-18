.extern _fopen
.extern _fputs
.extern _fprintf
.extern _fclose
.extern _printf
.extern _puts
.extern _putc
.extern _malloc
.extern _calloc
.extern _free
.extern _memset
.extern _strcmp
.extern _strncmp
.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
	xorl    %eax, %eax
_me$ = -4
	subl    $4, %esp
	movl    $8, %eax
	push    %eax
	call    _malloc
	addl    $4, %esp
	movl    %eax, _me$(%ebp)
	movl    $..String.0, %eax
	movl    %eax, %edx
	movl    _me$(%ebp), %eax
	addl    $0, %eax
	movl    %edx, (%eax)
	movl    $12, %eax
	movl    %eax, %edx
	movl    _me$(%ebp), %eax
	addl    $4, %eax
	movl    %edx, (%eax)
	movl    _me$(%ebp), %eax
	addl    $4, %eax
	movl    (%eax), %eax
	push    %eax
	movl    _me$(%ebp), %eax
	addl    $0, %eax
	movl    (%eax), %eax
	push    %eax
	movl    $..String.1, %eax
	push    %eax
	call    _printf
	addl    $4, %esp
	movl    %ebp, %esp
	pop     %ebp
	ret
..String.0:
	.string "Teddy"
..String.1:
	.string "Name %s, Age %i"
