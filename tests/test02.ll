; ModuleID = 'my_module'
source_filename = "my_module"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@printslit_str = private unnamed_addr constant [24 x i8] c"Test starts, wowwww...\0A\00"
@printslit_str.1 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.2 = private unnamed_addr constant [28 x i8] c"1. program starts with run\0A\00"
@printslit_str.3 = private unnamed_addr constant [22 x i8] c"okay -- 5pts (5/100)\0A\00"
@printslit_str.4 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.5 = private unnamed_addr constant [21 x i8] c"2. print statements\0A\00"
@print_int = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_float = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.6 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (10/100)\0A\00"
@printslit_str.7 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.8 = private unnamed_addr constant [33 x i8] c"3. extern function declarations\0A\00"
@print_int.9 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_float.10 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@print_int.11 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.12 = private unnamed_addr constant [24 x i8] c"okay -- 10pts (20/100)\0A\00"
@printslit_str.13 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.14 = private unnamed_addr constant [32 x i8] c"4. ref types - local variables\0A\00"
@print_int.15 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.16 = private unnamed_addr constant [21 x i8] c"1 increases by 1 is\0A\00"
@print_int.17 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_float.18 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.19 = private unnamed_addr constant [22 x i8] c"5 multiplied by 2 is\0A\00"
@print_float.20 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.21 = private unnamed_addr constant [24 x i8] c"okay -- 10pts (30/100)\0A\00"
@printslit_str.22 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.23 = private unnamed_addr constant [36 x i8] c"5. ref types - function parameters\0A\00"
@printslit_str.24 = private unnamed_addr constant [43 x i8] c"0 increases by 2 (call @inc two times) is\0A\00"
@print_int.25 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.26 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (35/100)\0A\00"
@printslit_str.27 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.28 = private unnamed_addr constant [19 x i8] c"6. Function calls\0A\00"
@printslit_str.29 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (40/100)\0A\00"
@printslit_str.30 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.31 = private unnamed_addr constant [22 x i8] c"7. Return statements\0A\00"
@print_float.32 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.33 = private unnamed_addr constant [34 x i8] c"5 divided by 2 is (calling div2)\0A\00"
@print_float.34 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.35 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (45/100)\0A\00"
@printslit_str.36 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.37 = private unnamed_addr constant [23 x i8] c"8. Integer operations\0A\00"
@printslit_str.38 = private unnamed_addr constant [6 x i8] c"g is\0A\00"
@print_int.39 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.40 = private unnamed_addr constant [6 x i8] c"h is\0A\00"
@print_int.41 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.42 = private unnamed_addr constant [13 x i8] c"g and 0 is:\0A\00"
@print_int.43 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.44 = private unnamed_addr constant [12 x i8] c"h or 0 is:\0A\00"
@print_int.45 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.46 = private unnamed_addr constant [11 x i8] c"not g is:\0A\00"
@print_int.47 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.48 = private unnamed_addr constant [13 x i8] c"minus h is:\0A\00"
@print_int.49 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.50 = private unnamed_addr constant [24 x i8] c"okay -- 10pts (55/100)\0A\00"
@printslit_str.51 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.52 = private unnamed_addr constant [30 x i8] c"9. floating point operations\0A\00"
@printslit_str.53 = private unnamed_addr constant [6 x i8] c"i is\0A\00"
@print_float.54 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.55 = private unnamed_addr constant [6 x i8] c"j is\0A\00"
@print_float.56 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.57 = private unnamed_addr constant [10 x i8] c"i - j is\0A\00"
@print_float.58 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.59 = private unnamed_addr constant [10 x i8] c"i * j is\0A\00"
@print_float.60 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.61 = private unnamed_addr constant [14 x i8] c"i and 0.0 is\0A\00"
@print_int.62 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.63 = private unnamed_addr constant [13 x i8] c"j or 0.0 is\0A\00"
@print_int.64 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.65 = private unnamed_addr constant [10 x i8] c"not i is\0A\00"
@print_int.66 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.67 = private unnamed_addr constant [12 x i8] c"minus j is\0A\00"
@print_float.68 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.69 = private unnamed_addr constant [24 x i8] c"okay -- 10pts (65/100)\0A\00"
@printslit_str.70 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.71 = private unnamed_addr constant [24 x i8] c"10. if/else statements\0A\00"
@print_int.72 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_int.73 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_int.74 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_float.75 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@print_float.76 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.77 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (70/100)\0A\00"
@printslit_str.78 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.79 = private unnamed_addr constant [22 x i8] c"11. while statements\0A\00"
@print_int.80 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.81 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (75/100)\0A\00"
@printslit_str.82 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.83 = private unnamed_addr constant [22 x i8] c"12. block statements\0A\00"
@print_int.84 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_int.85 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.86 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (80/100)\0A\00"
@printslit_str.87 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.88 = private unnamed_addr constant [23 x i8] c"13. assign statements\0A\00"
@printslit_str.89 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (85/100)\0A\00"
@printslit_str.90 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.91 = private unnamed_addr constant [22 x i8] c"14. noalias ref type\0A\00"
@printslit_str.92 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (90/100)\0A\00"
@printslit_str.93 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.94 = private unnamed_addr constant [36 x i8] c"15. integer comparison expressions\0A\00"
@print_int.95 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_int.96 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.97 = private unnamed_addr constant [23 x i8] c"okay -- 5pts (95/100)\0A\00"
@printslit_str.98 = private unnamed_addr constant [2 x i8] c"\0A\00"
@printslit_str.99 = private unnamed_addr constant [43 x i8] c"16. floating-point comparison expressions\0A\00"
@print_int.100 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@print_int.101 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@printslit_str.102 = private unnamed_addr constant [24 x i8] c"okay -- 5pts (100/100)\0A\00"
@printslit_str.103 = private unnamed_addr constant [2 x i8] c"\0A\00"

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

declare i32 @sushi(i32, i32)

declare float @ramen(float* noalias, float* noalias)

define void @inc(i32*) {
entry:
  %a = alloca i32*
  store i32* %0, i32** %a
  %tmp = load i32*, i32** %a
  %tmp1 = load i32, i32* %tmp
  %add = add nsw i32 %tmp1, 1
  %tmp2 = load i32*, i32** %a
  %tmp3 = load i32, i32* %tmp2
  %add4 = add nsw i32 %tmp3, 1
  %ptr = load i32*, i32** %a
  %varval = load i32, i32* %ptr
  store i32 %add4, i32* %ptr
  ret void
}

define float @div2(float) {
entry:
  %f = alloca float
  store float %0, float* %f
  %tmp = load float, float* %f
  %fdiv = fdiv float %tmp, 2.000000e+00
  ret float %fdiv
}

define float @fsum(float, float) {
entry:
  %a = alloca float
  store float %0, float* %a
  %b = alloca float
  store float %1, float* %b
  %tmp = load float, float* %a
  %tmp1 = load float, float* %b
  %fadd = fadd float %tmp, %tmp1
  ret float %fadd
}

define i32 @iftest1(i32) {
entry:
  %n = alloca i32
  store i32 %0, i32* %n
  %tmp = load i32, i32* %n
  %icmp = icmp eq i32 %tmp, 0
  %cmp_value = zext i1 %icmp to i32
  %tobool = icmp ne i32 %cmp_value, 0
  br i1 %tobool, label %if_yes, label %if_no

if_yes:                                           ; preds = %entry
  ret i32 0

if_no:                                            ; preds = %entry
  %tmp4 = load i32, i32* %n
  %icmp5 = icmp sgt i32 %tmp4, 0
  %cmp_value6 = zext i1 %icmp5 to i32
  %tobool7 = icmp ne i32 %cmp_value6, 0
  br i1 %tobool7, label %if_yes1, label %if_no2

if_yes1:                                          ; preds = %if_no
  ret i32 1

if_no2:                                           ; preds = %if_no
  ret i32 -1
}

define void @iftest2(float* noalias, float* noalias) {
entry:
  %a = alloca float*
  store float* %0, float** %a
  %b = alloca float*
  store float* %1, float** %b
  %tmp = load float*, float** %a
  %tmp1 = load float, float* %tmp
  %tmp2 = load float*, float** %b
  %tmp3 = load float, float* %tmp2
  %cmp = fcmp ugt float %tmp1, %tmp3
  %cmp_value = zext i1 %cmp to i32
  %tobool = icmp ne i32 %cmp_value, 0
  br i1 %tobool, label %if_yes, label %if_no

if_yes:                                           ; preds = %entry
  %tmp4 = load float*, float** %a
  %tmp5 = load float, float* %tmp4
  %tmp6 = load float*, float** %b
  %tmp7 = load float, float* %tmp6
  %fsub = fsub float %tmp5, %tmp7
  %tmp8 = load float*, float** %a
  %tmp9 = load float, float* %tmp8
  %tmp10 = load float*, float** %b
  %tmp11 = load float, float* %tmp10
  %fsub12 = fsub float %tmp9, %tmp11
  %ptr = load float*, float** %a
  %varval = load float, float* %ptr
  store float %fsub12, float* %ptr
  br label %if_end

if_no:                                            ; preds = %entry
  %tmp13 = load float*, float** %b
  %tmp14 = load float, float* %tmp13
  %tmp15 = load float*, float** %a
  %tmp16 = load float, float* %tmp15
  %fsub17 = fsub float %tmp14, %tmp16
  %tmp18 = load float*, float** %b
  %tmp19 = load float, float* %tmp18
  %tmp20 = load float*, float** %a
  %tmp21 = load float, float* %tmp20
  %fsub22 = fsub float %tmp19, %tmp21
  %ptr23 = load float*, float** %b
  %varval24 = load float, float* %ptr23
  store float %fsub22, float* %ptr23
  br label %if_end

if_end:                                           ; preds = %if_no, %if_yes
  ret void
}

define void @whiletest(i32*) {
entry:
  %n = alloca i32*
  store i32* %0, i32** %n
  br label %while_cond

while_cond:                                       ; preds = %while_loop, %entry
  %tmp = load i32*, i32** %n
  %tmp1 = load i32, i32* %tmp
  %icmp = icmp sgt i32 %tmp1, 0
  %cmp_value = zext i1 %icmp to i32
  %tobool = icmp ne i32 %cmp_value, 0
  br i1 %tobool, label %while_loop, label %while_end

while_loop:                                       ; preds = %while_cond
  %tmp2 = load i32*, i32** %n
  %tmp3 = load i32, i32* %tmp2
  %sub = sub i32 %tmp3, 1
  %tmp4 = load i32*, i32** %n
  %tmp5 = load i32, i32* %tmp4
  %sub6 = sub i32 %tmp5, 1
  %ptr = load i32*, i32** %n
  %varval = load i32, i32* %ptr
  store i32 %sub6, i32* %ptr
  br label %while_cond

while_end:                                        ; preds = %while_cond
  ret void
}

define i32 @run() {
entry:
  %printslit = call i32 (...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @printslit_str, i32 0, i32 0))
  %printslit1 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.1, i32 0, i32 0))
  %printslit2 = call i32 (...) @printf(i8* getelementptr inbounds ([28 x i8], [28 x i8]* @printslit_str.2, i32 0, i32 0))
  %printslit3 = call i32 (...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @printslit_str.3, i32 0, i32 0))
  %printslit4 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.4, i32 0, i32 0))
  %printslit5 = call i32 (...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @printslit_str.5, i32 0, i32 0))
  %a = alloca i32
  store i32 1, i32* %a
  %tmp = load i32, i32* %a
  %print = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int, i32 0, i32 0), i32 %tmp)
  %fa = alloca float
  store float 5.000000e+00, float* %fa
  %tmp6 = load float, float* %fa
  %fpext = fpext float %tmp6 to double
  %print7 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float, i32 0, i32 0), double %fpext)
  %printslit8 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.6, i32 0, i32 0))
  %printslit9 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.7, i32 0, i32 0))
  %printslit10 = call i32 (...) @printf(i8* getelementptr inbounds ([33 x i8], [33 x i8]* @printslit_str.8, i32 0, i32 0))
  %getarg = call i32 @getarg(i32 0)
  %c1 = alloca i32
  store i32 %getarg, i32* %c1
  %tmp11 = load i32, i32* %c1
  %print12 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.9, i32 0, i32 0), i32 %tmp11)
  %getargf = call float @getargf(i32 1)
  %c2 = alloca float
  store float %getargf, float* %c2
  %tmp13 = load float, float* %c2
  %fpext14 = fpext float %tmp13 to double
  %print15 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.10, i32 0, i32 0), double %fpext14)
  %getarg16 = call i32 @getarg(i32 0)
  %getarg17 = call i32 @getarg(i32 1)
  %add = add nsw i32 %getarg16, %getarg17
  %getargf18 = call float @getargf(i32 2)
  %conv = sitofp i32 %add to float
  %fadd = fadd float %conv, %getargf18
  %conv19 = fptosi float %fadd to i32
  %c3 = alloca i32
  store i32 %conv19, i32* %c3
  %tmp20 = load i32, i32* %c3
  %print21 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.11, i32 0, i32 0), i32 %tmp20)
  %printslit22 = call i32 (...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @printslit_str.12, i32 0, i32 0))
  %printslit23 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.13, i32 0, i32 0))
  %printslit24 = call i32 (...) @printf(i8* getelementptr inbounds ([32 x i8], [32 x i8]* @printslit_str.14, i32 0, i32 0))
  %tmp25 = load i32, i32* %a
  %print26 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.15, i32 0, i32 0), i32 %tmp25)
  %printslit27 = call i32 (...) @printf(i8* getelementptr inbounds ([21 x i8], [21 x i8]* @printslit_str.16, i32 0, i32 0))
  %tmp28 = load i32, i32* %a
  %b = alloca i32*
  store i32* %a, i32** %b
  %tmp29 = load i32*, i32** %b
  %tmp30 = load i32, i32* %tmp29
  %add31 = add nsw i32 %tmp30, 1
  %tmp32 = load i32*, i32** %b
  %tmp33 = load i32, i32* %tmp32
  %add34 = add nsw i32 %tmp33, 1
  %ptr = load i32*, i32** %b
  %varval = load i32, i32* %ptr
  store i32 %add34, i32* %ptr
  %tmp35 = load i32, i32* %a
  %print36 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.17, i32 0, i32 0), i32 %tmp35)
  %tmp37 = load float, float* %fa
  %fpext38 = fpext float %tmp37 to double
  %print39 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.18, i32 0, i32 0), double %fpext38)
  %printslit40 = call i32 (...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @printslit_str.19, i32 0, i32 0))
  %tmp41 = load float, float* %fa
  %fb = alloca float*
  store float* %fa, float** %fb
  %tmp42 = load float*, float** %fb
  %tmp43 = load float, float* %tmp42
  %fmul = fmul float %tmp43, 2.000000e+00
  %tmp44 = load float*, float** %fb
  %tmp45 = load float, float* %tmp44
  %fmul46 = fmul float %tmp45, 2.000000e+00
  %ptr47 = load float*, float** %fb
  %varval48 = load float, float* %ptr47
  store float %fmul46, float* %ptr47
  %tmp49 = load float, float* %fa
  %fpext50 = fpext float %tmp49 to double
  %print51 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.20, i32 0, i32 0), double %fpext50)
  %printslit52 = call i32 (...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @printslit_str.21, i32 0, i32 0))
  %printslit53 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.22, i32 0, i32 0))
  %printslit54 = call i32 (...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @printslit_str.23, i32 0, i32 0))
  %c = alloca i32
  store i32 0, i32* %c
  %printslit55 = call i32 (...) @printf(i8* getelementptr inbounds ([43 x i8], [43 x i8]* @printslit_str.24, i32 0, i32 0))
  call void @inc(i32* %c)
  call void @inc(i32* %c)
  %tmp56 = load i32, i32* %c
  %print57 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.25, i32 0, i32 0), i32 %tmp56)
  %printslit58 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.26, i32 0, i32 0))
  %printslit59 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.27, i32 0, i32 0))
  %printslit60 = call i32 (...) @printf(i8* getelementptr inbounds ([19 x i8], [19 x i8]* @printslit_str.28, i32 0, i32 0))
  %printslit61 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.29, i32 0, i32 0))
  %printslit62 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.30, i32 0, i32 0))
  %printslit63 = call i32 (...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @printslit_str.31, i32 0, i32 0))
  %d = alloca float
  store float 5.000000e+00, float* %d
  %tmp64 = load float, float* %d
  %fpext65 = fpext float %tmp64 to double
  %print66 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.32, i32 0, i32 0), double %fpext65)
  %printslit67 = call i32 (...) @printf(i8* getelementptr inbounds ([34 x i8], [34 x i8]* @printslit_str.33, i32 0, i32 0))
  %tmp68 = load float, float* %d
  %div2 = call float @div2(float %tmp68)
  %e = alloca float
  store float %div2, float* %e
  %tmp69 = load float, float* %e
  %fpext70 = fpext float %tmp69 to double
  %print71 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.34, i32 0, i32 0), double %fpext70)
  %printslit72 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.35, i32 0, i32 0))
  %printslit73 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.36, i32 0, i32 0))
  %printslit74 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.37, i32 0, i32 0))
  %g = alloca i32
  store i32 14, i32* %g
  %printslit75 = call i32 (...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @printslit_str.38, i32 0, i32 0))
  %tmp76 = load i32, i32* %g
  %print77 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.39, i32 0, i32 0), i32 %tmp76)
  %h = alloca i32
  store i32 4, i32* %h
  %printslit78 = call i32 (...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @printslit_str.40, i32 0, i32 0))
  %tmp79 = load i32, i32* %h
  %print80 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.41, i32 0, i32 0), i32 %tmp79)
  %tmp81 = load i32, i32* %g
  %cmp_ptr = alloca i1
  store i1 false, i1* %cmp_ptr
  %tobool = icmp ne i32 %tmp81, 0
  br i1 %tobool, label %"and(br1)", label %"and(end)"

"and(br1)":                                       ; preds = %entry
  br label %"and(end)"

"and(end)":                                       ; preds = %"and(br1)", %entry
  %cmp_value = load i1, i1* %cmp_ptr
  %cmp_value82 = zext i1 %cmp_value to i32
  %gand0 = alloca i32
  store i32 %cmp_value82, i32* %gand0
  %printslit83 = call i32 (...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @printslit_str.42, i32 0, i32 0))
  %tmp84 = load i32, i32* %gand0
  %print85 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.43, i32 0, i32 0), i32 %tmp84)
  %tmp86 = load i32, i32* %h
  %cmp_ptr87 = alloca i1
  store i1 true, i1* %cmp_ptr87
  %tobool88 = icmp ne i32 %tmp86, 0
  br i1 %tobool88, label %"or(end)", label %"or(br2)"

"or(br2)":                                        ; preds = %"and(end)"
  store i1 false, i1* %cmp_ptr87
  br label %"or(end)"

"or(end)":                                        ; preds = %"or(br2)", %"and(end)"
  %cmp_value89 = load i1, i1* %cmp_ptr87
  %cmp_value90 = zext i1 %cmp_value89 to i32
  %hor0 = alloca i32
  store i32 %cmp_value90, i32* %hor0
  %printslit91 = call i32 (...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @printslit_str.44, i32 0, i32 0))
  %tmp92 = load i32, i32* %hor0
  %print93 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.45, i32 0, i32 0), i32 %tmp92)
  %tmp94 = load i32, i32* %g
  %tobool95 = icmp ne i32 %tmp94, 0
  %lnot = xor i1 %tobool95, true
  %lnot.ext = zext i1 %lnot to i32
  %notg = alloca i32
  store i32 %lnot.ext, i32* %notg
  %printslit96 = call i32 (...) @printf(i8* getelementptr inbounds ([11 x i8], [11 x i8]* @printslit_str.46, i32 0, i32 0))
  %tmp97 = load i32, i32* %notg
  %print98 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.47, i32 0, i32 0), i32 %tmp97)
  %tmp99 = load i32, i32* %h
  %sub = sub i32 0, %tmp99
  %minush = alloca i32
  store i32 %sub, i32* %minush
  %printslit100 = call i32 (...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @printslit_str.48, i32 0, i32 0))
  %tmp101 = load i32, i32* %minush
  %print102 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.49, i32 0, i32 0), i32 %tmp101)
  %printslit103 = call i32 (...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @printslit_str.50, i32 0, i32 0))
  %printslit104 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.51, i32 0, i32 0))
  %printslit105 = call i32 (...) @printf(i8* getelementptr inbounds ([30 x i8], [30 x i8]* @printslit_str.52, i32 0, i32 0))
  %i = alloca float
  store float 4.500000e+00, float* %i
  %printslit106 = call i32 (...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @printslit_str.53, i32 0, i32 0))
  %tmp107 = load float, float* %i
  %fpext108 = fpext float %tmp107 to double
  %print109 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.54, i32 0, i32 0), double %fpext108)
  %j = alloca float
  store float 0xC008CCCCC0000000, float* %j
  %printslit110 = call i32 (...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @printslit_str.55, i32 0, i32 0))
  %tmp111 = load float, float* %j
  %fpext112 = fpext float %tmp111 to double
  %print113 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.56, i32 0, i32 0), double %fpext112)
  %printslit114 = call i32 (...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @printslit_str.57, i32 0, i32 0))
  %tmp115 = load float, float* %i
  %tmp116 = load float, float* %j
  %fsub = fsub float %tmp115, %tmp116
  %fpext117 = fpext float %fsub to double
  %print118 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.58, i32 0, i32 0), double %fpext117)
  %printslit119 = call i32 (...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @printslit_str.59, i32 0, i32 0))
  %tmp120 = load float, float* %i
  %tmp121 = load float, float* %j
  %fmul122 = fmul float %tmp120, %tmp121
  %fpext123 = fpext float %fmul122 to double
  %print124 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.60, i32 0, i32 0), double %fpext123)
  %tmp125 = load float, float* %i
  %cmp_ptr126 = alloca i1
  store i1 false, i1* %cmp_ptr126
  %tobool127 = fcmp une float %tmp125, 0.000000e+00
  br i1 %tobool127, label %"fand(br1)", label %"fand(end)"

"fand(br1)":                                      ; preds = %"or(end)"
  br label %"fand(end)"

"fand(end)":                                      ; preds = %"fand(br1)", %"or(end)"
  %cmp_value128 = load i1, i1* %cmp_ptr126
  %cmp_value129 = zext i1 %cmp_value128 to i32
  %iand0 = alloca i32
  store i32 %cmp_value129, i32* %iand0
  %printslit130 = call i32 (...) @printf(i8* getelementptr inbounds ([14 x i8], [14 x i8]* @printslit_str.61, i32 0, i32 0))
  %tmp131 = load i32, i32* %iand0
  %print132 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.62, i32 0, i32 0), i32 %tmp131)
  %tmp133 = load float, float* %j
  %cmp_ptr134 = alloca i1
  store i1 true, i1* %cmp_ptr134
  %tobool135 = fcmp une float %tmp133, 0.000000e+00
  br i1 %tobool135, label %"for(end)", label %"for(br2)"

"for(br2)":                                       ; preds = %"fand(end)"
  store i1 false, i1* %cmp_ptr134
  br label %"for(end)"

"for(end)":                                       ; preds = %"for(br2)", %"fand(end)"
  %cmp_value136 = load i1, i1* %cmp_ptr134
  %cmp_value137 = zext i1 %cmp_value136 to i32
  %jor0 = alloca i32
  store i32 %cmp_value137, i32* %jor0
  %printslit138 = call i32 (...) @printf(i8* getelementptr inbounds ([13 x i8], [13 x i8]* @printslit_str.63, i32 0, i32 0))
  %tmp139 = load i32, i32* %jor0
  %print140 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.64, i32 0, i32 0), i32 %tmp139)
  %tmp141 = load float, float* %i
  %tobool142 = fcmp one float %tmp141, 0.000000e+00
  %lnot143 = xor i1 %tobool142, true
  %lnot.ext144 = zext i1 %lnot143 to i32
  %noti = alloca i32
  store i32 %lnot.ext144, i32* %noti
  %printslit145 = call i32 (...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @printslit_str.65, i32 0, i32 0))
  %tmp146 = load i32, i32* %noti
  %print147 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.66, i32 0, i32 0), i32 %tmp146)
  %tmp148 = load float, float* %j
  %fsub149 = fsub float 0.000000e+00, %tmp148
  %minusj = alloca float
  store float %fsub149, float* %minusj
  %printslit150 = call i32 (...) @printf(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @printslit_str.67, i32 0, i32 0))
  %tmp151 = load float, float* %minusj
  %fpext152 = fpext float %tmp151 to double
  %print153 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.68, i32 0, i32 0), double %fpext152)
  %printslit154 = call i32 (...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @printslit_str.69, i32 0, i32 0))
  %printslit155 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.70, i32 0, i32 0))
  %printslit156 = call i32 (...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @printslit_str.71, i32 0, i32 0))
  %k = alloca i32
  store i32 5, i32* %k
  %l = alloca i32
  store i32 -6, i32* %l
  %ll = alloca i32
  store i32 0, i32* %ll
  %tmp157 = load i32, i32* %k
  %iftest1 = call i32 @iftest1(i32 %tmp157)
  %print158 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.72, i32 0, i32 0), i32 %iftest1)
  %tmp159 = load i32, i32* %l
  %iftest1160 = call i32 @iftest1(i32 %tmp159)
  %print161 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.73, i32 0, i32 0), i32 %iftest1160)
  %tmp162 = load i32, i32* %ll
  %iftest1163 = call i32 @iftest1(i32 %tmp162)
  %print164 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.74, i32 0, i32 0), i32 %iftest1163)
  %m = alloca float
  store float 7.000000e+00, float* %m
  %n = alloca float
  store float 5.000000e+00, float* %n
  call void @iftest2(float* %m, float* %n)
  call void @iftest2(float* %m, float* %n)
  %tmp165 = load float, float* %m
  %fpext166 = fpext float %tmp165 to double
  %print167 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.75, i32 0, i32 0), double %fpext166)
  %tmp168 = load float, float* %n
  %fpext169 = fpext float %tmp168 to double
  %print170 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.76, i32 0, i32 0), double %fpext169)
  %printslit171 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.77, i32 0, i32 0))
  %printslit172 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.78, i32 0, i32 0))
  %printslit173 = call i32 (...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @printslit_str.79, i32 0, i32 0))
  %o = alloca i32
  store i32 10, i32* %o
  call void @whiletest(i32* %o)
  %tmp174 = load i32, i32* %o
  %print175 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.80, i32 0, i32 0), i32 %tmp174)
  %printslit176 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.81, i32 0, i32 0))
  %printslit177 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.82, i32 0, i32 0))
  %printslit178 = call i32 (...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @printslit_str.83, i32 0, i32 0))
  %p = alloca i32
  store i32 10, i32* %p
  %tmp179 = load i32, i32* %p
  %print180 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.84, i32 0, i32 0), i32 %tmp179)
  %tmp181 = load i32, i32* %p
  %q = alloca i32*
  store i32* %p, i32** %q
  %tmp182 = load i32*, i32** %q
  %tmp183 = load i32, i32* %tmp182
  %sub184 = sub i32 %tmp183, 2
  %tmp185 = load i32*, i32** %q
  %tmp186 = load i32, i32* %tmp185
  %sub187 = sub i32 %tmp186, 2
  %ptr188 = load i32*, i32** %q
  %varval189 = load i32, i32* %ptr188
  store i32 %sub187, i32* %ptr188
  %tmp190 = load i32*, i32** %q
  %tmp191 = load i32, i32* %tmp190
  %sub192 = sub i32 %tmp191, 2
  %tmp193 = load i32*, i32** %q
  %tmp194 = load i32, i32* %tmp193
  %sub195 = sub i32 %tmp194, 2
  %ptr196 = load i32*, i32** %q
  %varval197 = load i32, i32* %ptr196
  store i32 %sub195, i32* %ptr196
  %tmp198 = load i32, i32* %p
  %print199 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.85, i32 0, i32 0), i32 %tmp198)
  %printslit200 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.86, i32 0, i32 0))
  %printslit201 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.87, i32 0, i32 0))
  %printslit202 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.88, i32 0, i32 0))
  %printslit203 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.89, i32 0, i32 0))
  %printslit204 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.90, i32 0, i32 0))
  %printslit205 = call i32 (...) @printf(i8* getelementptr inbounds ([22 x i8], [22 x i8]* @printslit_str.91, i32 0, i32 0))
  %printslit206 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.92, i32 0, i32 0))
  %printslit207 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.93, i32 0, i32 0))
  %printslit208 = call i32 (...) @printf(i8* getelementptr inbounds ([36 x i8], [36 x i8]* @printslit_str.94, i32 0, i32 0))
  %x = alloca i32
  store i32 1, i32* %x
  %y = alloca i32
  store i32 6, i32* %y
  %z = alloca i32
  store i32 -1, i32* %z
  %w = alloca i32
  store i32 -5, i32* %w
  %tmp209 = load i32, i32* %x
  %tmp210 = load i32, i32* %y
  %icmp = icmp slt i32 %tmp209, %tmp210
  %cmp_value211 = zext i1 %icmp to i32
  %tmp212 = load i32, i32* %z
  %tmp213 = load i32, i32* %w
  %icmp214 = icmp slt i32 %tmp212, %tmp213
  %cmp_value215 = zext i1 %icmp214 to i32
  %cmp_ptr216 = alloca i1
  store i1 false, i1* %cmp_ptr216
  %tobool217 = icmp ne i32 %cmp_value211, 0
  br i1 %tobool217, label %"and(br1)218", label %"and(end)220"

"and(br1)218":                                    ; preds = %"for(end)"
  %tobool221 = icmp ne i32 %cmp_value215, 0
  br i1 %tobool221, label %"and(br2)219", label %"and(end)220"

"and(br2)219":                                    ; preds = %"and(br1)218"
  store i1 true, i1* %cmp_ptr216
  br label %"and(end)220"

"and(end)220":                                    ; preds = %"and(br2)219", %"and(br1)218", %"for(end)"
  %cmp_value222 = load i1, i1* %cmp_ptr216
  %cmp_value223 = zext i1 %cmp_value222 to i32
  %print224 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.95, i32 0, i32 0), i32 %cmp_value223)
  %tmp225 = load i32, i32* %x
  %tmp226 = load i32, i32* %y
  %icmp227 = icmp sgt i32 %tmp225, %tmp226
  %cmp_value228 = zext i1 %icmp227 to i32
  %tmp229 = load i32, i32* %z
  %tmp230 = load i32, i32* %w
  %icmp231 = icmp sgt i32 %tmp229, %tmp230
  %cmp_value232 = zext i1 %icmp231 to i32
  %cmp_ptr233 = alloca i1
  store i1 true, i1* %cmp_ptr233
  %tobool234 = icmp ne i32 %cmp_value228, 0
  br i1 %tobool234, label %"or(end)237", label %"or(br1)235"

"or(br1)235":                                     ; preds = %"and(end)220"
  %tobool238 = icmp ne i32 %cmp_value232, 0
  br i1 %tobool238, label %"or(end)237", label %"or(br2)236"

"or(br2)236":                                     ; preds = %"or(br1)235"
  store i1 false, i1* %cmp_ptr233
  br label %"or(end)237"

"or(end)237":                                     ; preds = %"or(br2)236", %"or(br1)235", %"and(end)220"
  %cmp_value239 = load i1, i1* %cmp_ptr233
  %cmp_value240 = zext i1 %cmp_value239 to i32
  %print241 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.96, i32 0, i32 0), i32 %cmp_value240)
  %printslit242 = call i32 (...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @printslit_str.97, i32 0, i32 0))
  %printslit243 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.98, i32 0, i32 0))
  %printslit244 = call i32 (...) @printf(i8* getelementptr inbounds ([43 x i8], [43 x i8]* @printslit_str.99, i32 0, i32 0))
  %fx = alloca float
  store float 0x3FE6666660000000, float* %fx
  %fy = alloca float
  store float 0x3FE8E2EB20000000, float* %fy
  %fw = alloca float
  store float 0x3FE166ADC0000000, float* %fw
  %fz = alloca float
  store float 0x3FDC054F80000000, float* %fz
  %tmp245 = load float, float* %fx
  %tmp246 = load float, float* %fy
  %cmp = fcmp ult float %tmp245, %tmp246
  %cmp_value247 = zext i1 %cmp to i32
  %tmp248 = load float, float* %fz
  %tmp249 = load float, float* %fw
  %cmp250 = fcmp ult float %tmp248, %tmp249
  %cmp_value251 = zext i1 %cmp250 to i32
  %cmp_ptr252 = alloca i1
  store i1 false, i1* %cmp_ptr252
  %tobool253 = icmp ne i32 %cmp_value247, 0
  br i1 %tobool253, label %"and(br1)254", label %"and(end)256"

"and(br1)254":                                    ; preds = %"or(end)237"
  %tobool257 = icmp ne i32 %cmp_value251, 0
  br i1 %tobool257, label %"and(br2)255", label %"and(end)256"

"and(br2)255":                                    ; preds = %"and(br1)254"
  store i1 true, i1* %cmp_ptr252
  br label %"and(end)256"

"and(end)256":                                    ; preds = %"and(br2)255", %"and(br1)254", %"or(end)237"
  %cmp_value258 = load i1, i1* %cmp_ptr252
  %cmp_value259 = zext i1 %cmp_value258 to i32
  %print260 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.100, i32 0, i32 0), i32 %cmp_value259)
  %tmp261 = load float, float* %fx
  %tmp262 = load float, float* %fy
  %cmp263 = fcmp ugt float %tmp261, %tmp262
  %cmp_value264 = zext i1 %cmp263 to i32
  %tmp265 = load float, float* %fz
  %tmp266 = load float, float* %fw
  %cmp267 = fcmp ugt float %tmp265, %tmp266
  %cmp_value268 = zext i1 %cmp267 to i32
  %cmp_ptr269 = alloca i1
  store i1 true, i1* %cmp_ptr269
  %tobool270 = icmp ne i32 %cmp_value264, 0
  br i1 %tobool270, label %"or(end)273", label %"or(br1)271"

"or(br1)271":                                     ; preds = %"and(end)256"
  %tobool274 = icmp ne i32 %cmp_value268, 0
  br i1 %tobool274, label %"or(end)273", label %"or(br2)272"

"or(br2)272":                                     ; preds = %"or(br1)271"
  store i1 false, i1* %cmp_ptr269
  br label %"or(end)273"

"or(end)273":                                     ; preds = %"or(br2)272", %"or(br1)271", %"and(end)256"
  %cmp_value275 = load i1, i1* %cmp_ptr269
  %cmp_value276 = zext i1 %cmp_value275 to i32
  %print277 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_int.101, i32 0, i32 0), i32 %cmp_value276)
  %printslit278 = call i32 (...) @printf(i8* getelementptr inbounds ([24 x i8], [24 x i8]* @printslit_str.102, i32 0, i32 0))
  %printslit279 = call i32 (...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @printslit_str.103, i32 0, i32 0))
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
