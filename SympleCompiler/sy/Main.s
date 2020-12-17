.extern _fopen
.extern _fputs
.extern _fprintf
.extern _fclose
.extern _printf
.extern _puts
.extern _putc
.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
_num$ = -2
	subl    $2, %esp
	movw    $45, %ax
	movw    %ax, _num$(%ebp)
_num2$ = -6
	subl    $4, %esp
	movw    _num$(%ebp), %ax
	movswl  %ax, %eax
	movl    %eax, _num2$(%ebp)
	movl    %ebp, %esp
	pop     %ebp
	ret
