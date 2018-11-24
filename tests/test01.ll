; ModuleID = 'my_module'
source_filename = "my_module"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@print_int = private unnamed_addr constant [4 x i8] c"%d\0A\00"

declare i32 @printf(...) local_unnamed_addr

define void @inc(i32* noalias nocapture, i32* noalias nocapture) local_unnamed_addr {
entry:
  %tmp1 = load i32, i32* %0, align 4
  %add4 = add i32 %tmp1, 1
  store i32 %add4, i32* %0, align 4
  %tmp6 = load i32, i32* %1, align 4
  %add10 = add i32 %tmp6, 1
  store i32 %add10, i32* %1, align 4
  %print = tail call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int, i64 0, i64 0), i32 %add4)
  ret void
}

define i32 @run() local_unnamed_addr {
entry:
  %a = alloca i32, align 4
  store i32 2, i32* %a, align 4
  call void @inc(i32* nonnull %a, i32* nonnull %a)
  ret i32 15
}

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
