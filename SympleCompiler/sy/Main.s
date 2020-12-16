.extern _fopen
.extern _fputs
.extern _fclose
.global _main
_main:
	push    %ebp
	movl    %esp, %ebp
_test_txt$ = -4
	subl    $4, %esp
	movl    $..String.0, %eax
	push    %eax
	movl    $..String.1, %eax
	push    %eax
	call    _fopen
	addl    $4, %esp
	movl    %eax, _test_txt$(%ebp)
	movl    _test_txt$(%ebp), %eax
	push    %eax
	movl    $..String.2, %eax
	push    %eax
	call    _fputs
	addl    $4, %esp
	movl    _test_txt$(%ebp), %eax
	push    %eax
	call    _fclose
	addl    $4, %esp
	movl    %ebp, %esp
	pop     %ebp
	ret
..String.0:
	.string "w"
..String.1:
	.string "sy/Test.txt"
..String.2:
	.string "This test works!"
