; ModuleID = 'simple5.ll'
source_filename = "simple5.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !7 {
entry:
  %retval = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %un = alloca i32, align 4
  %x = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  tail call void @__coverage__(i32 4, i32 7)
  call void @llvm.dbg.declare(metadata i32* %y, metadata !11, metadata !DIExpression()), !dbg !12
  tail call void @__coverage__(i32 4, i32 7)
  store i32 1, i32* %y, align 4, !dbg !12
  tail call void @__coverage__(i32 4, i32 14)
  call void @llvm.dbg.declare(metadata i32* %z, metadata !13, metadata !DIExpression()), !dbg !14
  tail call void @__coverage__(i32 4, i32 14)
  store i32 1, i32* %z, align 4, !dbg !14
  tail call void @__coverage__(i32 5, i32 7)
  call void @llvm.dbg.declare(metadata i32* %un, metadata !15, metadata !DIExpression()), !dbg !16
  tail call void @__coverage__(i32 6, i32 3)
  %call = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32* %un), !dbg !17
  tail call void @__coverage__(i32 7, i32 7)
  call void @llvm.dbg.declare(metadata i32* %x, metadata !18, metadata !DIExpression()), !dbg !19
  tail call void @__coverage__(i32 7, i32 11)
  %0 = load i32, i32* %un, align 4, !dbg !20
  tail call void @__coverage__(i32 7, i32 14)
  %add = add nsw i32 %0, 1, !dbg !21
  tail call void @__coverage__(i32 7, i32 7)
  store i32 %add, i32* %x, align 4, !dbg !19
  tail call void @__coverage__(i32 8, i32 7)
  %1 = load i32, i32* %y, align 4, !dbg !22
  tail call void @__coverage__(i32 8, i32 11)
  %2 = load i32, i32* %x, align 4, !dbg !23
  tail call void @__coverage__(i32 8, i32 9)
  tail call void @__sanitize__(i32 %2, i32 8, i32 9)
  %div = sdiv i32 %1, %2, !dbg !24
  tail call void @__coverage__(i32 8, i32 5)
  store i32 %div, i32* %z, align 4, !dbg !25
  tail call void @__coverage__(i32 9, i32 3)
  ret i32 0, !dbg !26
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @__isoc99_scanf(i8*, ...) #2

declare void @__coverage__(i32, i32)

declare void @__sanitize__(i32, i32, i32)

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 12.0.1-19ubuntu3", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "simple5.c", directory: "/home/shank/code/purdue/ece695_hss/LLVMPlayground/part4_instrumentation/DivZeroInstrument/test")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
!7 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 3, type: !8, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DILocalVariable(name: "y", scope: !7, file: !1, line: 4, type: !10)
!12 = !DILocation(line: 4, column: 7, scope: !7)
!13 = !DILocalVariable(name: "z", scope: !7, file: !1, line: 4, type: !10)
!14 = !DILocation(line: 4, column: 14, scope: !7)
!15 = !DILocalVariable(name: "un", scope: !7, file: !1, line: 5, type: !10)
!16 = !DILocation(line: 5, column: 7, scope: !7)
!17 = !DILocation(line: 6, column: 3, scope: !7)
!18 = !DILocalVariable(name: "x", scope: !7, file: !1, line: 7, type: !10)
!19 = !DILocation(line: 7, column: 7, scope: !7)
!20 = !DILocation(line: 7, column: 11, scope: !7)
!21 = !DILocation(line: 7, column: 14, scope: !7)
!22 = !DILocation(line: 8, column: 7, scope: !7)
!23 = !DILocation(line: 8, column: 11, scope: !7)
!24 = !DILocation(line: 8, column: 9, scope: !7)
!25 = !DILocation(line: 8, column: 5, scope: !7)
!26 = !DILocation(line: 9, column: 3, scope: !7)
