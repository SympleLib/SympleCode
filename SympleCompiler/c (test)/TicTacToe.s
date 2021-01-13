	.text
	.def	 @feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 1
	.file	"TicTacToe.c"
	.def	 _CreateBoard;
	.scl	2;
	.type	32;
	.endef
	.globl	_CreateBoard                    # -- Begin function CreateBoard
	.p2align	4, 0x90
_CreateBoard:                           # @CreateBoard
# %bb.0:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$12, %esp
	movl	$9, (%esp)
	calll	_malloc
	movl	%eax, -4(%ebp)
	movl	$0, -8(%ebp)
LBB0_1:                                 # =>This Inner Loop Header: Depth=1
	cmpl	$9, -8(%ebp)
	jge	LBB0_4
# %bb.2:                                #   in Loop: Header=BB0_1 Depth=1
	movl	-4(%ebp), %eax
	movl	-8(%ebp), %ecx
	movb	$35, (%eax,%ecx)
# %bb.3:                                #   in Loop: Header=BB0_1 Depth=1
	movl	-8(%ebp), %eax
	addl	$1, %eax
	movl	%eax, -8(%ebp)
	jmp	LBB0_1
LBB0_4:
	movl	-4(%ebp), %eax
	addl	$12, %esp
	popl	%ebp
	retl
                                        # -- End function
	.def	 _PrintBoard;
	.scl	2;
	.type	32;
	.endef
	.globl	_PrintBoard                     # -- Begin function PrintBoard
	.p2align	4, 0x90
_PrintBoard:                            # @PrintBoard
# %bb.0:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%edi
	pushl	%esi
	subl	$28, %esp
	movl	8(%ebp), %eax
	movl	8(%ebp), %ecx
	movsbl	2(%ecx), %ecx
	movl	8(%ebp), %edx
	movsbl	1(%edx), %edx
	movl	8(%ebp), %esi
	movsbl	(%esi), %esi
	leal	"??_C@_0O@LEOCEHGI@?$CFc?5?$HM?5?$CFc?5?$HM?5?$CFc?6?$AA@", %edi
	movl	%edi, (%esp)
	movl	%esi, 4(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 12(%esp)
	movl	%eax, -12(%ebp)                 # 4-byte Spill
	calll	_printf
	movl	8(%ebp), %ecx
	movsbl	5(%ecx), %ecx
	movl	8(%ebp), %edx
	movsbl	4(%edx), %edx
	movl	8(%ebp), %esi
	movsbl	3(%esi), %esi
	leal	"??_C@_0O@LEOCEHGI@?$CFc?5?$HM?5?$CFc?5?$HM?5?$CFc?6?$AA@", %edi
	movl	%edi, (%esp)
	movl	%esi, 4(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 12(%esp)
	movl	%eax, -16(%ebp)                 # 4-byte Spill
	calll	_printf
	movl	8(%ebp), %ecx
	movsbl	8(%ecx), %ecx
	movl	8(%ebp), %edx
	movsbl	7(%edx), %edx
	movl	8(%ebp), %esi
	movsbl	6(%esi), %esi
	leal	"??_C@_0O@LEOCEHGI@?$CFc?5?$HM?5?$CFc?5?$HM?5?$CFc?6?$AA@", %edi
	movl	%edi, (%esp)
	movl	%esi, 4(%esp)
	movl	%edx, 8(%esp)
	movl	%ecx, 12(%esp)
	movl	%eax, -20(%ebp)                 # 4-byte Spill
	calll	_printf
	addl	$28, %esp
	popl	%esi
	popl	%edi
	popl	%ebp
	retl
                                        # -- End function
	.def	 _GetWinner;
	.scl	2;
	.type	32;
	.endef
	.globl	_GetWinner                      # -- Begin function GetWinner
	.p2align	4, 0x90
_GetWinner:                             # @GetWinner
# %bb.0:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$8, %esp
	movl	8(%ebp), %eax
	movl	8(%ebp), %ecx
	movb	(%ecx), %dl
	movb	%dl, -5(%ebp)
	movsbl	%dl, %ecx
	cmpl	$35, %ecx
	je	LBB2_13
# %bb.1:
	movl	8(%ebp), %eax
	movsbl	1(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_4
# %bb.2:
	movl	8(%ebp), %eax
	movsbl	2(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_4
# %bb.3:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_4:
	movl	8(%ebp), %eax
	movsbl	3(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_7
# %bb.5:
	movl	8(%ebp), %eax
	movsbl	6(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_7
# %bb.6:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_7:
	movl	8(%ebp), %eax
	movsbl	4(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_10
# %bb.8:
	movl	8(%ebp), %eax
	movsbl	8(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_10
# %bb.9:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_10:
	jmp	LBB2_11
LBB2_11:
	jmp	LBB2_12
LBB2_12:
	jmp	LBB2_13
LBB2_13:
	movl	8(%ebp), %eax
	movb	4(%eax), %cl
	movb	%cl, -5(%ebp)
	movsbl	%cl, %eax
	cmpl	$35, %eax
	je	LBB2_26
# %bb.14:
	movl	8(%ebp), %eax
	movsbl	3(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_17
# %bb.15:
	movl	8(%ebp), %eax
	movsbl	5(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_17
# %bb.16:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_17:
	movl	8(%ebp), %eax
	movsbl	1(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_20
# %bb.18:
	movl	8(%ebp), %eax
	movsbl	7(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_20
# %bb.19:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_20:
	movl	8(%ebp), %eax
	movsbl	2(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_23
# %bb.21:
	movl	8(%ebp), %eax
	movsbl	6(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_23
# %bb.22:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_23:
	jmp	LBB2_24
LBB2_24:
	jmp	LBB2_25
LBB2_25:
	jmp	LBB2_26
LBB2_26:
	movl	8(%ebp), %eax
	movb	8(%eax), %cl
	movb	%cl, -5(%ebp)
	movsbl	%cl, %eax
	cmpl	$35, %eax
	je	LBB2_35
# %bb.27:
	movl	8(%ebp), %eax
	movsbl	2(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_30
# %bb.28:
	movl	8(%ebp), %eax
	movsbl	5(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_30
# %bb.29:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_30:
	movl	8(%ebp), %eax
	movsbl	6(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_33
# %bb.31:
	movl	8(%ebp), %eax
	movsbl	7(%eax), %eax
	movsbl	-5(%ebp), %ecx
	cmpl	%ecx, %eax
	jne	LBB2_33
# %bb.32:
	movsbl	-5(%ebp), %eax
	cmpl	$88, %eax
	movl	$1, %eax
	movl	$2, %ecx
	cmovel	%eax, %ecx
	movl	%ecx, -4(%ebp)
	jmp	LBB2_36
LBB2_33:
	jmp	LBB2_34
LBB2_34:
	jmp	LBB2_35
LBB2_35:
	movl	$0, -4(%ebp)
LBB2_36:
	movl	-4(%ebp), %eax
	addl	$8, %esp
	popl	%ebp
	retl
                                        # -- End function
	.def	 _main;
	.scl	2;
	.type	32;
	.endef
	.globl	_main                           # -- Begin function main
	.p2align	4, 0x90
_main:                                  # @main
# %bb.0:
	pushl	%ebp
	movl	%esp, %ebp
	pushl	%esi
	subl	$36, %esp
	movl	$0, -8(%ebp)
	movb	$1, -9(%ebp)
	calll	_CreateBoard
	movl	%eax, -16(%ebp)
LBB3_1:                                 # =>This Loop Header: Depth=1
                                        #     Child Loop BB3_3 Depth 2
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	calll	_GetWinner
	cmpl	$0, %eax
	setne	%cl
	xorb	$-1, %cl
	testb	$1, %cl
	jne	LBB3_2
	jmp	LBB3_8
LBB3_2:                                 #   in Loop: Header=BB3_1 Depth=1
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	calll	_PrintBoard
	movl	$0, -20(%ebp)
LBB3_3:                                 #   Parent Loop BB3_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	cmpl	$1, -20(%ebp)
	movb	$1, %al
	movb	%al, -21(%ebp)                  # 1-byte Spill
	jl	LBB3_5
# %bb.4:                                #   in Loop: Header=BB3_3 Depth=2
	cmpl	$9, -20(%ebp)
	setg	%al
	movb	%al, -21(%ebp)                  # 1-byte Spill
LBB3_5:                                 #   in Loop: Header=BB3_3 Depth=2
	movb	-21(%ebp), %al                  # 1-byte Reload
	testb	$1, %al
	jne	LBB3_6
	jmp	LBB3_7
LBB3_6:                                 #   in Loop: Header=BB3_3 Depth=2
	movb	-9(%ebp), %al
	testb	$1, %al
	movl	$88, %ecx
	movl	$79, %edx
	cmovnel	%ecx, %edx
	leal	"??_C@_0CJ@NGHCHCPI@Player?5?8?$CFc?8?0?5pick?5a?5number?5from?5@", %ecx
	movl	%ecx, (%esp)
	movl	%edx, 4(%esp)
	calll	_printf
	leal	"??_C@_02IKAHHCAI@?$CFi?$AA@", %ecx
	movl	%ecx, (%esp)
	leal	-20(%ebp), %ecx
	movl	%ecx, 4(%esp)
	movl	%eax, -28(%ebp)                 # 4-byte Spill
	calll	_scanf_s
	jmp	LBB3_3
LBB3_7:                                 #   in Loop: Header=BB3_1 Depth=1
	movb	-9(%ebp), %al
	testb	$1, %al
	movl	$88, %ecx
	movl	$79, %edx
	cmovnel	%ecx, %edx
                                        # kill: def $dl killed $dl killed $edx
	movl	-16(%ebp), %ecx
	movl	-20(%ebp), %esi
	subl	$1, %esi
	movb	%dl, (%ecx,%esi)
	movb	-9(%ebp), %al
	xorb	$-1, %al
	andb	$1, %al
	movb	%al, -9(%ebp)
	jmp	LBB3_1
LBB3_8:
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	calll	_PrintBoard
	movl	-16(%ebp), %eax
	movl	%eax, (%esp)
	calll	_GetWinner
	movl	-8(%ebp), %ecx
	movl	%eax, -32(%ebp)                 # 4-byte Spill
	movl	%ecx, %eax
	addl	$36, %esp
	popl	%esi
	popl	%ebp
	retl
                                        # -- End function
	.section	.rdata,"dr",discard,"??_C@_0O@LEOCEHGI@?$CFc?5?$HM?5?$CFc?5?$HM?5?$CFc?6?$AA@"
	.globl	"??_C@_0O@LEOCEHGI@?$CFc?5?$HM?5?$CFc?5?$HM?5?$CFc?6?$AA@" # @"??_C@_0O@LEOCEHGI@?$CFc?5?$HM?5?$CFc?5?$HM?5?$CFc?6?$AA@"
"??_C@_0O@LEOCEHGI@?$CFc?5?$HM?5?$CFc?5?$HM?5?$CFc?6?$AA@":
	.asciz	"%c | %c | %c\n"

	.section	.rdata,"dr",discard,"??_C@_0CJ@NGHCHCPI@Player?5?8?$CFc?8?0?5pick?5a?5number?5from?5@"
	.globl	"??_C@_0CJ@NGHCHCPI@Player?5?8?$CFc?8?0?5pick?5a?5number?5from?5@" # @"??_C@_0CJ@NGHCHCPI@Player?5?8?$CFc?8?0?5pick?5a?5number?5from?5@"
"??_C@_0CJ@NGHCHCPI@Player?5?8?$CFc?8?0?5pick?5a?5number?5from?5@":
	.asciz	"Player '%c', pick a number from 1-10...\n"

	.section	.rdata,"dr",discard,"??_C@_02IKAHHCAI@?$CFi?$AA@"
	.globl	"??_C@_02IKAHHCAI@?$CFi?$AA@"   # @"??_C@_02IKAHHCAI@?$CFi?$AA@"
"??_C@_02IKAHHCAI@?$CFi?$AA@":
	.asciz	"%i"

	.addrsig
	.addrsig_sym _CreateBoard
	.addrsig_sym _malloc
	.addrsig_sym _PrintBoard
	.addrsig_sym _printf
	.addrsig_sym _GetWinner
	.addrsig_sym _scanf_s
