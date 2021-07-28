; ModuleID = 'Code.sy'
source_filename = "Code.sy"

%Human = type <{ i8*, i32 }>

@0 = private unnamed_addr constant [8 x i8] c"Treidex\00", align 1
@1 = private unnamed_addr constant [43 x i8] c"Name: '%s', Coolness (100%% Accurate): %i\0A\00", align 1

declare i32 @printf(i8*, i8*, i32)

define i32 @Run() {
  %best = alloca %Human, align 8
  %1 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 0
  store i8* getelementptr inbounds ([8 x i8], [8 x i8]* @0, i32 0, i32 0), i8** %1, align 8
  %2 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 1
  store i32 99999, i32* %2, align 4
  %3 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 0
  %4 = load i8*, i8** %3, align 8
  %5 = getelementptr inbounds %Human, %Human* %best, i32 0, i32 1
  %6 = load i32, i32* %5, align 4
  %7 = call i32 @printf(i8* getelementptr inbounds ([43 x i8], [43 x i8]* @1, i32 0, i32 0), i8* %4, i32 %6)
  ret i32 0
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
