; ModuleID = 'Code.sy'
source_filename = "Code.sy"

declare void @_putws(i16*)

define i32 @Run() {
  %x = alloca i32, align 4
  store i32 5, i32* %x, align 4
  %1 = load i32, i32* %x, align 4
  %2 = icmp eq i32 %1, 35
  br i1 %2, label %3, label %6

3:                                                ; preds = %0
  %4 = alloca [13 x i16], align 2
  store [13 x i16] [i16 72, i16 101, i16 108, i16 108, i16 111, i16 44, i16 32, i16 119, i16 111, i16 114, i16 108, i16 100, i16 0], [13 x i16]* %4, align 2
  %5 = bitcast [13 x i16]* %4 to i16*
  call void @_putws(i16* %5)
  br label %9

6:                                                ; preds = %0
  %7 = alloca [13 x i16], align 2
  store [13 x i16] [i16 72, i16 101, i16 108, i16 108, i16 111, i16 32, i16 87, i16 111, i16 114, i16 108, i16 100, i16 33, i16 0], [13 x i16]* %7, align 2
  %8 = bitcast [13 x i16]* %7 to i16*
  call void @_putws(i16* %8)
  br label %9

9:                                                ; preds = %6, %3
  ret i32 0
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
