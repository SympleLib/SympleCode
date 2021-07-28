; ModuleID = 'Code.sy'
source_filename = "Code.sy"

declare void @MessageBoxW(i8*, i16*, i16*, i32)

define i64 @Run() {
  %1 = alloca [5 x i16], align 2
  store [5 x i16] [i16 84, i16 101, i16 115, i16 116, i16 0], [5 x i16]* %1, align 2
  %2 = bitcast [5 x i16]* %1 to i16*
  %3 = alloca [7 x i16], align 2
  store [7 x i16] [i16 72, i16 101, i16 108, i16 108, i16 111, i16 33, i16 0], [7 x i16]* %3, align 2
  %4 = bitcast [7 x i16]* %3 to i16*
  call void @MessageBoxW(i8* null, i16* %2, i16* %4, i32 0)
  ret i64 0
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
