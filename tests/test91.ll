; ModuleID = 'my_module'
source_filename = "my_module"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@printslit_str = private unnamed_addr constant [9 x i8] c"fib(5):\0A\00"
@print_int = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.1 = private unnamed_addr constant [11 x i8] c"fib(5)+1:\0A\00"
@print_int.2 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.3 = private unnamed_addr constant [17 x i8] c"something else:\0A\00"
@print_int.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00"

declare i32 @printf(...)

define i32 @getarg(i32) {
entry:
  %load_from_array = extractelement <3 x float> <float 1.000000e+00, float 2.000000e+00, float 3.000000e+00>, i32 %0
  %conv = fptosi float %load_from_array to i32
  ret i32 %conv
}

define float @getargf(i32) {
entry:
  %load_from_array = extractelement <3 x float> <float 1.000000e+00, float 2.000000e+00, float 3.000000e+00>, i32 %0
  ret float %load_from_array
}

define i32 @fib(i32) {
entry:
  %n = alloca i32
  store i32 %0, i32* %n
  %tmp = load i32, i32* %n
  %icmp = icmp slt i32 %tmp, 2
  %cmp_value = zext i1 %icmp to i32
  %tobool = icmp ne i32 %cmp_value, 0
  br i1 %tobool, label %if_yes, label %if_end

if_yes:                                           ; preds = %entry
  %tmp3 = load i32, i32* %n
  %icmp4 = icmp eq i32 %tmp3, 0
  %cmp_value5 = zext i1 %icmp4 to i32
  %tobool6 = icmp ne i32 %cmp_value5, 0
  br i1 %tobool6, label %if_yes1, label %if_no

if_end:                                           ; preds = %entry
  %tmp7 = load i32, i32* %n
  %sub = sub i32 %tmp7, 1
  %fib = call i32 @fib(i32 %sub)
  %a = alloca i32
  store i32 %fib, i32* %a
  %tmp8 = load i32, i32* %n
  %sub9 = sub i32 %tmp8, 2
  %fib10 = call i32 @fib(i32 %sub9)
  %b = alloca i32
  store i32 %fib10, i32* %b
  %tmp11 = load i32, i32* %a
  %tmp12 = load i32, i32* %b
  %add = add i32 %tmp11, %tmp12
  ret i32 %add

if_yes1:                                          ; preds = %if_yes
  ret i32 0

if_no:                                            ; preds = %if_yes
  ret i32 1
}

define void @inc(i32*) {
entry:
  %n = alloca i32*
  store i32* %0, i32** %n
  %tmp = load i32*, i32** %n
  %tmp1 = load i32, i32* %tmp
  %add = add i32 %tmp1, 1
  %tmp2 = load i32*, i32** %n
  %tmp3 = load i32, i32* %tmp2
  %add4 = add i32 %tmp3, 1
  %ptr = load i32*, i32** %n
  %varval = load i32, i32* %ptr
  store i32 %add4, i32* %ptr
  ret void
}

define void @things(i32*) {
entry:
  %n = alloca i32*
  store i32* %0, i32** %n
  br label %while_cond

while_cond:                                       ; preds = %while_loop, %entry
  %tmp = load i32*, i32** %n
  %tmp1 = load i32, i32* %tmp
  %icmp = icmp sgt i32 %tmp1, 100
  %cmp_value = zext i1 %icmp to i32
  %tobool = icmp ne i32 %cmp_value, 0
  %lnot = xor i1 %tobool, true
  %lnot.ext = zext i1 %lnot to i32
  %tobool2 = icmp ne i32 %lnot.ext, 0
  br i1 %tobool2, label %while_loop, label %while_end

while_loop:                                       ; preds = %while_cond
  %tmp3 = load i32*, i32** %n
  %tmp4 = load i32, i32* %tmp3
  %tmp5 = load i32*, i32** %n
  %tmp6 = load i32, i32* %tmp5
  %mul = mul i32 %tmp4, %tmp6
  %sub = sub i32 %mul, 2
  %tmp7 = load i32*, i32** %n
  %tmp8 = load i32, i32* %tmp7
  %tmp9 = load i32*, i32** %n
  %tmp10 = load i32, i32* %tmp9
  %mul11 = mul i32 %tmp8, %tmp10
  %sub12 = sub i32 %mul11, 2
  %ptr = load i32*, i32** %n
  %varval = load i32, i32* %ptr
  store i32 %sub12, i32* %ptr
  br label %while_cond

while_end:                                        ; preds = %while_cond
  ret void
}

define i32 @run() {
entry:
  %printslit = call i32 (...) @printf(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @printslit_str, i32 0, i32 0))
  %fib = call i32 @fib(i32 5)
  %val = alloca i32
  store i32 %fib, i32* %val
  %tmp = load i32, i32* %val
  %print = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int, i32 0, i32 0), i32 %tmp)
  %printslit1 = call i32 (...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @printslit_str.1, i32 0, i32 0))
  call void @inc(i32* %val)
  %tmp2 = load i32, i32* %val
  %print3 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.2, i32 0, i32 0), i32 %tmp2)
  %printslit4 = call i32 (...) @printf(i8* getelementptr inbounds ([17 x i8], [17 x i8]* @printslit_str.3, i32 0, i32 0))
  call void @things(i32* %val)
  %tmp5 = load i32, i32* %val
  %print6 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.4, i32 0, i32 0), i32 %tmp5)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
