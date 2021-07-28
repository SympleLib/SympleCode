; ModuleID = 'Code.sy'
source_filename = "Code.sy"

@0 = private unnamed_addr constant [14 x i8] c"Hello, world!\00", align 1

declare i32 @puts(i16*)

define i32 @Run() {
  %1 = call i32 @puts(i16* bitcast ([14 x i8]* @0 to i16*))
  ret i32 %1
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
