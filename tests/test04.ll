; ModuleID = 'my_module'
source_filename = "my_module"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@print_int = private unnamed_addr constant [4 x i8] c"%d\0A\00"

declare i32 @printf(...)

declare void @cint_negation(i32, i32*)

declare void @cint_addition(i32, i32, i32*)

declare void @cint_subtraction(i32, i32, i32*)

declare void @cint_multiplication(i32, i32, i32*)

declare void @cint_division(i32, i32, i32*)

define i32 @run() {
entry:
  %a = alloca i32
  store i32 -2147483647, i32* %a
  %b = alloca i32
  store i32 -1, i32* %b
  %tmp = load i32, i32* %a
  %tmp1 = load i32, i32* %b
  %div = sdiv i32 %tmp, %tmp1
  %ptr = alloca i32
  store i32 %div, i32* %ptr
  call void @cint_division(i32 %tmp, i32 %tmp1, i32* %ptr)
  %c = alloca i32
  store i32 %div, i32* %c
  %tmp2 = load i32, i32* %c
  %print = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int, i32 0, i32 0), i32 %tmp2)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
