; ModuleID = 'simple6.c'
source_filename = "simple6.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !7 {
entry:
  %retval = alloca i32, align 4
  %u1 = alloca i32, align 4
  %u2 = alloca i32, align 4
  %d = alloca i32, align 4
  %d1 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %u1, metadata !11, metadata !DIExpression()), !dbg !12
  %call = call i32 @getchar(), !dbg !13
  store i32 %call, i32* %u1, align 4, !dbg !12
  call void @llvm.dbg.declare(metadata i32* %u2, metadata !14, metadata !DIExpression()), !dbg !15
  store i32 4, i32* %u2, align 4, !dbg !15
  call void @llvm.dbg.declare(metadata i32* %d, metadata !16, metadata !DIExpression()), !dbg !17
  %0 = load i32, i32* %u1, align 4, !dbg !18
  %cmp = icmp ne i32 %0, 0, !dbg !20
  br i1 %cmp, label %if.then, label %if.else, !dbg !21

if.then:                                          ; preds = %entry
  %1 = load i32, i32* %u2, align 4, !dbg !22
  %2 = load i32, i32* %u1, align 4, !dbg !24
  %div = sdiv i32 %1, %2, !dbg !25
  store i32 %div, i32* %d, align 4, !dbg !26
  br label %if.end, !dbg !27

if.else:                                          ; preds = %entry
  call void @llvm.dbg.declare(metadata i32* %d1, metadata !28, metadata !DIExpression()), !dbg !30
  %3 = load i32, i32* %u2, align 4, !dbg !31
  %4 = load i32, i32* %u1, align 4, !dbg !32
  %add = add nsw i32 %4, 1, !dbg !33
  %div2 = sdiv i32 %3, %add, !dbg !34
  store i32 %div2, i32* %d1, align 4, !dbg !30
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  ret i32 0, !dbg !35
}

; Function Attrs: nofree nosync nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare dso_local i32 @getchar() #2

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nofree nosync nounwind readnone speculatable willreturn }
attributes #2 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "Ubuntu clang version 12.0.1-19ubuntu3", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "simple6.c", directory: "/home/shank/code/purdue/ece695_hss/LLVMPlayground/part4_instrumentation/DivZeroInstrument/test")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
!7 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 3, type: !8, scopeLine: 3, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DILocalVariable(name: "u1", scope: !7, file: !1, line: 4, type: !10)
!12 = !DILocation(line: 4, column: 7, scope: !7)
!13 = !DILocation(line: 4, column: 12, scope: !7)
!14 = !DILocalVariable(name: "u2", scope: !7, file: !1, line: 5, type: !10)
!15 = !DILocation(line: 5, column: 7, scope: !7)
!16 = !DILocalVariable(name: "d", scope: !7, file: !1, line: 5, type: !10)
!17 = !DILocation(line: 5, column: 15, scope: !7)
!18 = !DILocation(line: 6, column: 7, scope: !19)
!19 = distinct !DILexicalBlock(scope: !7, file: !1, line: 6, column: 7)
!20 = !DILocation(line: 6, column: 10, scope: !19)
!21 = !DILocation(line: 6, column: 7, scope: !7)
!22 = !DILocation(line: 7, column: 9, scope: !23)
!23 = distinct !DILexicalBlock(scope: !19, file: !1, line: 6, column: 16)
!24 = !DILocation(line: 7, column: 14, scope: !23)
!25 = !DILocation(line: 7, column: 12, scope: !23)
!26 = !DILocation(line: 7, column: 7, scope: !23)
!27 = !DILocation(line: 8, column: 3, scope: !23)
!28 = !DILocalVariable(name: "d", scope: !29, file: !1, line: 9, type: !10)
!29 = distinct !DILexicalBlock(scope: !19, file: !1, line: 8, column: 10)
!30 = !DILocation(line: 9, column: 9, scope: !29)
!31 = !DILocation(line: 9, column: 13, scope: !29)
!32 = !DILocation(line: 9, column: 19, scope: !29)
!33 = !DILocation(line: 9, column: 22, scope: !29)
!34 = !DILocation(line: 9, column: 16, scope: !29)
!35 = !DILocation(line: 11, column: 3, scope: !7)
