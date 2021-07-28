; ModuleID = 'Code.sy'
source_filename = "Code.sy"

%Human = type <{ i16*, i32 }>

declare i32 @wprintf(i16*, i16*, i32)

define i32 @Run() {
  %imma-use-kebab-case = alloca i32, align 4
  %best = alloca %Human, align 8
  %1 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 0
  %2 = alloca [8 x i16], align 2
  store [8 x i16] [i16 84, i16 114, i16 101, i16 105, i16 100, i16 101, i16 120, i16 0], [8 x i16]* %2, align 2
  %3 = bitcast [8 x i16]* %2 to i16*
  store i16* %3, i16** %1, align 8
  %4 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 1
  store i32 99999, i32* %4, align 4
  %5 = alloca [43 x i16], align 2
  store [43 x i16] [i16 78, i16 97, i16 109, i16 101, i16 58, i16 32, i16 39, i16 37, i16 115, i16 39, i16 44, i16 32, i16 67, i16 111, i16 111, i16 108, i16 110, i16 101, i16 115, i16 115, i16 32, i16 40, i16 49, i16 48, i16 48, i16 37, i16 37, i16 32, i16 65, i16 99, i16 99, i16 117, i16 114, i16 97, i16 116, i16 101, i16 41, i16 58, i16 32, i16 37, i16 105, i16 10, i16 0], [43 x i16]* %5, align 2
  %6 = bitcast [43 x i16]* %5 to i16*
  %7 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 0
  %8 = load i16*, i16** %7, align 8
  %9 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 1
  %10 = load i32, i32* %9, align 4
  %11 = call i32 @wprintf(i16* %6, i16* %8, i32 %10)
  ret i32 0
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
