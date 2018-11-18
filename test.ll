; ModuleID = 'my_module'
source_filename = "my_module"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@printslit_str = private unnamed_addr constant [27 x i8] c"Welcome to walkure world!\0A\00"
@print_int = private unnamed_addr constant [4 x i8] c"%d\0A\00"

declare i32 @printf(...)

define i32 @inc(i32) {
entry:
  ret i32 0
}

define i32 @run() {
entry:
  %a = alloca i32
  store i32 7, i32* %a
  %tmp = load i32, i32* %a
  %b = alloca i32*
  store i32* %a, i32** %b
  %tmp1 = load i32*, i32** %b
  %tmp2 = load i32, i32* %tmp1
  %add = add i32 %tmp2, 1
  %tmp3 = load i32*, i32** %b
  %tmp4 = load i32, i32* %tmp3
  %add5 = add i32 %tmp4, 1
  %ptr = load i32*, i32** %b
  store i32 %add5, i32* %ptr
  %c = alloca i32
  store i32 5, i32* %c
  %tmp6 = load i32, i32* %a
  %tmp7 = load i32, i32* %c
  %add8 = add i32 %tmp6, %tmp7
  %tmp9 = load i32, i32* %a
  %tmp10 = load i32, i32* %c
  %add11 = add i32 %tmp9, %tmp10
  store i32 %add11, i32* %a
  %printslit = call i32 (...) @printf(i8* getelementptr inbounds ([27 x i8], [27 x i8]* @printslit_str, i32 0, i32 0))
  %tmp12 = load i32*, i32** %b
  %tmp13 = load i32, i32* %tmp12
  %print = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int, i32 0, i32 0), i32 %tmp13)
  ret i32 15
}

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
