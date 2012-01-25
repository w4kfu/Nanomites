; ModuleID = 'test.c'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@.str = private unnamed_addr constant [11 x i8] c"A equal 42\00", align 1
@.str1 = private unnamed_addr constant [14 x i8] c"A is below 30\00", align 1
@.str2 = private unnamed_addr constant [15 x i8] c"A not equal 42\00", align 1

define void @other_func() nounwind {
  %a = alloca i8*, align 4
  store i8* null, i8** %a, align 4
  br label %1

; <label>:1                                       ; preds = %0
  call void @exit(i32 0) noreturn nounwind
  unreachable
                                                  ; No predecessors!
  ret void
}

declare void @exit(i32) noreturn nounwind

define i32 @main(i32 %argc, i8** %argv) nounwind {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i8**, align 4
  store i32 0, i32* %1
  store i32 %argc, i32* %2, align 4
  store i8** %argv, i8*** %3, align 4
  %4 = load i32* %2, align 4
  %5 = icmp eq i32 %4, 42
  br i1 %5, label %6, label %8

; <label>:6                                       ; preds = %0
  %7 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([11 x i8]* @.str, i32 0, i32 0))
  br label %16

; <label>:8                                       ; preds = %0
  %9 = load i32* %2, align 4
  %10 = icmp slt i32 %9, 30
  br i1 %10, label %11, label %13

; <label>:11                                      ; preds = %8
  %12 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([14 x i8]* @.str1, i32 0, i32 0))
  br label %15

; <label>:13                                      ; preds = %8
  %14 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([15 x i8]* @.str2, i32 0, i32 0))
  br label %15

; <label>:15                                      ; preds = %13, %11
  br label %16

; <label>:16                                      ; preds = %15, %6
  call void @other_func()
  call void asm sideeffect "int3", "~{dirflag},~{fpsr},~{flags}"() nounwind, !srcloc !0
  ret i32 0
}

declare i32 @printf(i8*, ...)

!0 = metadata !{i32 390}
