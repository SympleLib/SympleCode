; ModuleID = 'Code.sy'
source_filename = "Code.sy"

declare i8* @GetStdHandle(i32)

declare void @WriteConsoleW(i8*, i16*, i32, i32*)

define i32 @Run() {
  %stdout = alloca i8*, align 8
  %1 = call i8* @GetStdHandle(i32 -11)
  store i8* %1, i8** %stdout, align 8
  %2 = load i8*, i8** %stdout, align 8
  %3 = alloca [15 x i16], align 2
  store [15 x i16] [i16 72, i16 101, i16 108, i16 108, i16 111, i16 44, i16 32, i16 119, i16 111, i16 114, i16 108, i16 100, i16 33, i16 10, i16 0], [15 x i16]* %3, align 2
  %4 = bitcast [15 x i16]* %3 to i16*
  call void @WriteConsoleW(i8* %2, i16* %4, i32 14, i32* null)
  ret i32 0
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
