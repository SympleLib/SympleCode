; ModuleID = 'Code.sy'
source_filename = "Code.sy"

@0 = private unnamed_addr constant [13 x i8] c"Hello, world\00", align 1
@1 = private unnamed_addr constant [13 x i8] c"Hello World!\00", align 1

declare void @puts(i16*)

define i32 @Run() {
  %x = alloca i32, align 4
  store i32 5, i32* %x, align 4
  %1 = load i32, i32* %x, align 4
  %2 = icmp eq i32 %1, 35
  br i1 %2, label %3, label %4

3:                                                ; preds = %0
  call void @puts(i16* bitcast ([13 x i8]* @0 to i16*))
  br label %5

4:                                                ; preds = %0
  call void @puts(i16* bitcast ([13 x i8]* @1 to i16*))
  br label %5

5:                                                ; preds = %4, %3
  ret i32 0
}

!llvm.dbg.cu = !{!0}

!0 = distinct !DICompileUnit(language: DW_LANG_C, file: !1, producer: "SuperCode d1.0.0", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, debugInfoForProfiling: true)
!1 = !DIFile(filename: "Code.sy", directory: "")
!2 = !{}
