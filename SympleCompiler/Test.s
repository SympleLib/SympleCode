	.text
	.def	 @feat.00;
	.scl	3;
	.type	0;
	.endef
	.globl	@feat.00
.set @feat.00, 1
	.file	"Test.c"
	.def	 _main;
	.scl	2;
	.type	32;
	.endef
	.globl	_main                           # -- Begin function main
	.p2align	4, 0x90
_main:                                  # @main
Lfunc_begin0:
	.cv_func_id 0
	.cv_file	1 "D:\\Projects\\C++\\SympleLib\\SympleCode\\SympleCompiler\\bin\\Test.c" "081EFF71C877A3986D7BC5916B74D2B1" 1
	.cv_loc	0 1 4 0                         # bin/Test.c:4:0
	.cv_fpo_proc	_main 0
# %bb.0:
	pushl	%ebp
	.cv_fpo_pushreg	%ebp
	movl	%esp, %ebp
	.cv_fpo_setframe	%ebp
	subl	$8, %esp
	.cv_fpo_stackalloc	8
	.cv_fpo_endprologue
	movl	$0, -4(%ebp)
Ltmp0:
	leal	"??_C@_0O@GEHPLBPJ@Hello?0?5world?$CB?$AA@", %eax
	movl	%eax, (%esp)
	calll	_puts
	addl	$8, %esp
	popl	%ebp
	retl
Ltmp1:
	.cv_fpo_endproc
Lfunc_end0:
                                        # -- End function
	.section	.rdata,"dr",discard,"??_C@_0O@GEHPLBPJ@Hello?0?5world?$CB?$AA@"
	.globl	"??_C@_0O@GEHPLBPJ@Hello?0?5world?$CB?$AA@" # @"??_C@_0O@GEHPLBPJ@Hello?0?5world?$CB?$AA@"
"??_C@_0O@GEHPLBPJ@Hello?0?5world?$CB?$AA@":
	.asciz	"Hello, world!"

	.section	.debug$S,"dr"
	.p2align	2
	.long	4                               # Debug section magic
	.long	241
	.long	Ltmp3-Ltmp2                     # Subsection size
Ltmp2:
	.short	Ltmp5-Ltmp4                     # Record length
Ltmp4:
	.short	4412                            # Record kind: S_COMPILE3
	.long	0                               # Flags and language
	.short	7                               # CPUType
	.short	11                              # Frontend version
	.short	0
	.short	0
	.short	0
	.short	11000                           # Backend version
	.short	0
	.short	0
	.short	0
	.asciz	"clang version 11.0.0"          # Null-terminated compiler version string
	.p2align	2
Ltmp5:
Ltmp3:
	.p2align	2
	.cv_fpo_data	_main
	.long	241                             # Symbol subsection for main
	.long	Ltmp7-Ltmp6                     # Subsection size
Ltmp6:
	.short	Ltmp9-Ltmp8                     # Record length
Ltmp8:
	.short	4423                            # Record kind: S_GPROC32_ID
	.long	0                               # PtrParent
	.long	0                               # PtrEnd
	.long	0                               # PtrNext
	.long	Lfunc_end0-_main                # Code size
	.long	0                               # Offset after prologue
	.long	0                               # Offset before epilogue
	.long	4098                            # Function type index
	.secrel32	_main                   # Function section relative address
	.secidx	_main                           # Function section index
	.byte	0                               # Flags
	.asciz	"main"                          # Function name
	.p2align	2
Ltmp9:
	.short	Ltmp11-Ltmp10                   # Record length
Ltmp10:
	.short	4114                            # Record kind: S_FRAMEPROC
	.long	12                              # FrameSize
	.long	0                               # Padding
	.long	0                               # Offset of padding
	.long	0                               # Bytes of callee saved registers
	.long	0                               # Exception handler offset
	.short	0                               # Exception handler section
	.long	163840                          # Flags (defines frame register)
	.p2align	2
Ltmp11:
	.short	2                               # Record length
	.short	4431                            # Record kind: S_PROC_ID_END
Ltmp7:
	.p2align	2
	.cv_linetable	0, _main, Lfunc_end0
	.cv_filechecksums                       # File index to string table offset subsection
	.cv_stringtable                         # String table
	.long	241
	.long	Ltmp13-Ltmp12                   # Subsection size
Ltmp12:
	.short	Ltmp15-Ltmp14                   # Record length
Ltmp14:
	.short	4428                            # Record kind: S_BUILDINFO
	.long	4101                            # LF_BUILDINFO index
	.p2align	2
Ltmp15:
Ltmp13:
	.p2align	2
	.section	.debug$T,"dr"
	.p2align	2
	.long	4                               # Debug section magic
	# ArgList (0x1000)
	.short	0x6                             # Record length
	.short	0x1201                          # Record kind: LF_ARGLIST
	.long	0x0                             # NumArgs
	# Procedure (0x1001)
	.short	0xe                             # Record length
	.short	0x1008                          # Record kind: LF_PROCEDURE
	.long	0x74                            # ReturnType: int
	.byte	0x0                             # CallingConvention: NearC
	.byte	0x0                             # FunctionOptions
	.short	0x0                             # NumParameters
	.long	0x1000                          # ArgListType: ()
	# FuncId (0x1002)
	.short	0x12                            # Record length
	.short	0x1601                          # Record kind: LF_FUNC_ID
	.long	0x0                             # ParentScope
	.long	0x1001                          # FunctionType: int ()
	.asciz	"main"                          # Name
	.byte	243
	.byte	242
	.byte	241
	# StringId (0x1003)
	.short	0x3a                            # Record length
	.short	0x1605                          # Record kind: LF_STRING_ID
	.long	0x0                             # Id
	.asciz	"D:\\Projects\\C++\\SympleLib\\SympleCode\\SympleCompiler" # StringData
	# StringId (0x1004)
	.short	0x12                            # Record length
	.short	0x1605                          # Record kind: LF_STRING_ID
	.long	0x0                             # Id
	.asciz	"bin\\Test.c"                   # StringData
	.byte	241
	# BuildInfo (0x1005)
	.short	0x1a                            # Record length
	.short	0x1603                          # Record kind: LF_BUILDINFO
	.short	0x5                             # NumArgs
	.long	0x1003                          # Argument: D:\Projects\C++\SympleLib\SympleCode\SympleCompiler
	.long	0x0                             # Argument
	.long	0x1004                          # Argument: bin\Test.c
	.long	0x0                             # Argument
	.long	0x0                             # Argument
	.byte	242
	.byte	241
	.addrsig
	.addrsig_sym _puts
