	.file	"tictactoe.c"
	.text
	.globl	_CreateBoard
	.def	_CreateBoard;	.scl	2;	.type	32;	.endef
_CreateBoard:
LFB14:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$40, %esp
	movl	$9, (%esp)
	call	_malloc
	movl	%eax, -16(%ebp)
	movl	$0, -12(%ebp)
	jmp	L2
L3:
	movl	-12(%ebp), %edx
	movl	-16(%ebp), %eax
	addl	%edx, %eax
	movb	$35, (%eax)
	addl	$1, -12(%ebp)
L2:
	cmpl	$8, -12(%ebp)
	jle	L3
	movl	-16(%ebp), %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE14:
	.section .rdata,"dr"
LC0:
	.ascii "%c | %c | %c\12\0"
	.text
	.globl	_PrintBoard
	.def	_PrintBoard;	.scl	2;	.type	32;	.endef
_PrintBoard:
LFB15:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$24, %esp
	movl	8(%ebp), %eax
	addl	$2, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %ecx
	movl	8(%ebp), %eax
	addl	$1, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %edx
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$LC0, (%esp)
	call	_printf
	movl	8(%ebp), %eax
	addl	$5, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %ecx
	movl	8(%ebp), %eax
	addl	$4, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %edx
	movl	8(%ebp), %eax
	addl	$3, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$LC0, (%esp)
	call	_printf
	movl	8(%ebp), %eax
	addl	$8, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %ecx
	movl	8(%ebp), %eax
	addl	$7, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %edx
	movl	8(%ebp), %eax
	addl	$6, %eax
	movzbl	(%eax), %eax
	movsbl	%al, %eax
	movl	%ecx, 12(%esp)
	movl	%edx, 8(%esp)
	movl	%eax, 4(%esp)
	movl	$LC0, (%esp)
	call	_printf
	nop
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE15:
	.globl	_GetWinner
	.def	_GetWinner;	.scl	2;	.type	32;	.endef
_GetWinner:
LFB16:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	subl	$16, %esp
	movl	8(%ebp), %eax
	movzbl	(%eax), %eax
	movb	%al, -1(%ebp)
	cmpb	$35, -1(%ebp)
	je	L7
	movl	8(%ebp), %eax
	addl	$1, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L8
	movl	8(%ebp), %eax
	addl	$2, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L8
	cmpb	$88, -1(%ebp)
	jne	L9
	movl	$1, %eax
	jmp	L11
L9:
	movl	$2, %eax
	jmp	L11
L8:
	movl	8(%ebp), %eax
	addl	$3, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L12
	movl	8(%ebp), %eax
	addl	$6, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L12
	cmpb	$88, -1(%ebp)
	jne	L13
	movl	$1, %eax
	jmp	L11
L13:
	movl	$2, %eax
	jmp	L11
L12:
	movl	8(%ebp), %eax
	addl	$4, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L7
	movl	8(%ebp), %eax
	addl	$8, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L7
	cmpb	$88, -1(%ebp)
	jne	L15
	movl	$1, %eax
	jmp	L11
L15:
	movl	$2, %eax
	jmp	L11
L7:
	movl	8(%ebp), %eax
	movzbl	4(%eax), %eax
	movb	%al, -1(%ebp)
	cmpb	$35, -1(%ebp)
	je	L17
	movl	8(%ebp), %eax
	addl	$3, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L18
	movl	8(%ebp), %eax
	addl	$5, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L18
	cmpb	$88, -1(%ebp)
	jne	L19
	movl	$1, %eax
	jmp	L11
L19:
	movl	$2, %eax
	jmp	L11
L18:
	movl	8(%ebp), %eax
	addl	$1, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L21
	movl	8(%ebp), %eax
	addl	$7, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L21
	cmpb	$88, -1(%ebp)
	jne	L22
	movl	$1, %eax
	jmp	L11
L22:
	movl	$2, %eax
	jmp	L11
L21:
	movl	8(%ebp), %eax
	addl	$2, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L17
	movl	8(%ebp), %eax
	addl	$6, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L17
	cmpb	$88, -1(%ebp)
	jne	L24
	movl	$1, %eax
	jmp	L11
L24:
	movl	$2, %eax
	jmp	L11
L17:
	movl	8(%ebp), %eax
	movzbl	8(%eax), %eax
	movb	%al, -1(%ebp)
	cmpb	$35, -1(%ebp)
	je	L26
	movl	8(%ebp), %eax
	addl	$2, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L27
	movl	8(%ebp), %eax
	addl	$5, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L27
	cmpb	$88, -1(%ebp)
	jne	L28
	movl	$1, %eax
	jmp	L11
L28:
	movl	$2, %eax
	jmp	L11
L27:
	movl	8(%ebp), %eax
	addl	$6, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L26
	movl	8(%ebp), %eax
	addl	$7, %eax
	movzbl	(%eax), %eax
	cmpb	-1(%ebp), %al
	jne	L26
	cmpb	$88, -1(%ebp)
	jne	L30
	movl	$1, %eax
	jmp	L11
L30:
	movl	$2, %eax
	jmp	L11
L26:
	movl	$0, %eax
L11:
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE16:
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
	.align 4
LC1:
	.ascii "Player '%c', pick a number from 1-10...\12\0"
LC2:
	.ascii "%i\0"
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
LFB17:
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	andl	$-16, %esp
	subl	$32, %esp
	call	___main
	movb	$1, 31(%esp)
	call	_CreateBoard
	movl	%eax, 24(%esp)
	jmp	L33
L40:
	movl	24(%esp), %eax
	movl	%eax, (%esp)
	call	_PrintBoard
	movl	$0, 20(%esp)
	jmp	L34
L37:
	cmpb	$0, 31(%esp)
	je	L35
	movl	$88, %eax
	jmp	L36
L35:
	movl	$79, %eax
L36:
	movl	%eax, 4(%esp)
	movl	$LC1, (%esp)
	call	_printf
	leal	20(%esp), %eax
	movl	%eax, 4(%esp)
	movl	$LC2, (%esp)
	call	_scanf_s
L34:
	movl	20(%esp), %eax
	testl	%eax, %eax
	jle	L37
	movl	20(%esp), %eax
	cmpl	$9, %eax
	jg	L37
	movl	20(%esp), %eax
	leal	-1(%eax), %edx
	movl	24(%esp), %eax
	addl	%edx, %eax
	cmpb	$0, 31(%esp)
	je	L38
	movl	$88, %edx
	jmp	L39
L38:
	movl	$79, %edx
L39:
	movb	%dl, (%eax)
	movzbl	31(%esp), %eax
	testl	%eax, %eax
	setne	%al
	xorl	$1, %eax
	movzbl	%al, %eax
	movb	%al, 31(%esp)
	andb	$1, 31(%esp)
L33:
	movl	24(%esp), %eax
	movl	%eax, (%esp)
	call	_GetWinner
	testl	%eax, %eax
	je	L40
	movl	24(%esp), %eax
	movl	%eax, (%esp)
	call	_PrintBoard
	movl	24(%esp), %eax
	movl	%eax, (%esp)
	call	_GetWinner
	movl	$0, %eax
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
LFE17:
	.ident	"GCC: (MinGW.org GCC-6.3.0-1) 6.3.0"
	.def	_malloc;	.scl	2;	.type	32;	.endef
	.def	_printf;	.scl	2;	.type	32;	.endef
	.def	_scanf_s;	.scl	2;	.type	32;	.endef
