; ModuleID = 'Code.sy'
source_filename = "Code.sy"

declare void @MessageBoxW(i8*, i16*, i16*, i32)

define i64 @Run() {
  %1 = alloca [3 x i16], align 2
  store [3 x i16] [i16 -10179, i16 -8695, i16 0], [3 x i16]* %1, align 2
  %2 = bitcast [3 x i16]* %1 to i16*
  %3 = alloca [10 x i16], align 2
  store [10 x i16] [i16 73, i16 116, i16 32, i16 119, i16 111, i16 114, i16 107, i16 115, i16 33, i16 0], [10 x i16]* %3, align 2
  %4 = bitcast [10 x i16]* %3 to i16*
  call void @MessageBoxW(i8* null, i16* %2, i16* %4, i32 0)
  ret i64 0
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
