; ModuleID = 'my_module'
source_filename = "my_module"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@printslit_str = private unnamed_addr constant [10 x i8] c"initial:\0A\00"
@print_float = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@print_float.1 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@print_float.2 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@printslit_str.3 = private unnamed_addr constant [8 x i8] c"final:\0A\00"
@print_float.4 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@print_float.5 = private unnamed_addr constant [4 x i8] c"%f\0A\00"
@print_float.6 = private unnamed_addr constant [4 x i8] c"%f\0A\00"

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

define float @test1(float* noalias, float* noalias, float) {
entry:
  %a = alloca float*
  store float* %0, float** %a
  %b = alloca float*
  store float* %1, float** %b
  %c = alloca float
  store float %2, float* %c
  %x = alloca i32
  store i32 0, i32* %x
  %xy = alloca i32
  store i32 1, i32* %xy
  %xyz = alloca i32
  store i32 2, i32* %xyz
  br label %while_cond

while_cond:                                       ; preds = %if_end, %entry
  %tmp = load i32, i32* %xyz
  %icmp = icmp sgt i32 %tmp, 8
  %cmp_value = zext i1 %icmp to i32
  %tobool = icmp ne i32 %cmp_value, 0
  br i1 %tobool, label %while_loop, label %while_end

while_loop:                                       ; preds = %while_cond
  %tmp1 = load float*, float** %a
  %tmp2 = load float, float* %tmp1
  %tmp3 = load float*, float** %b
  %tmp4 = load float, float* %tmp3
  %fadd = fadd float %tmp2, %tmp4
  %tmp5 = load float, float* %c
  %tmp6 = load float*, float** %a
  %tmp7 = load float, float* %tmp6
  %fdiv = fdiv float %tmp5, %tmp7
  %tmp8 = load float*, float** %b
  %tmp9 = load float, float* %tmp8
  %fmul = fmul float %fdiv, %tmp9
  %fsub = fsub float %fadd, %fmul
  %tmp10 = load float*, float** %a
  %tmp11 = load float, float* %tmp10
  %tmp12 = load float*, float** %b
  %tmp13 = load float, float* %tmp12
  %fadd14 = fadd float %tmp11, %tmp13
  %tmp15 = load float, float* %c
  %tmp16 = load float*, float** %a
  %tmp17 = load float, float* %tmp16
  %fdiv18 = fdiv float %tmp15, %tmp17
  %tmp19 = load float*, float** %b
  %tmp20 = load float, float* %tmp19
  %fmul21 = fmul float %fdiv18, %tmp20
  %fsub22 = fsub float %fadd14, %fmul21
  %varval = load i32, i32* %x
  %conv = fptosi float %fsub22 to i32
  store i32 %conv, i32* %x
  %tmp23 = load float*, float** %a
  %tmp24 = load float, float* %tmp23
  %tmp25 = load float*, float** %b
  %tmp26 = load float, float* %tmp25
  %cmp = fcmp ueq float %tmp24, %tmp26
  %cmp_value27 = zext i1 %cmp to i32
  %tmp28 = load float*, float** %a
  %tmp29 = load float, float* %tmp28
  %tmp30 = load float*, float** %b
  %tmp31 = load float, float* %tmp30
  %cmp32 = fcmp ueq float %tmp29, %tmp31
  %cmp_value33 = zext i1 %cmp32 to i32
  %varval34 = load i32, i32* %xy
  store i32 %cmp_value33, i32* %xy
  %tmp35 = load i32, i32* %xyz
  %tmp36 = load i32, i32* %xy
  %sub = sub i32 0, %tmp36
  %icmp37 = icmp sgt i32 %tmp35, %sub
  %cmp_value38 = zext i1 %icmp37 to i32
  %tmp39 = load float*, float** %a
  %tmp40 = load float, float* %tmp39
  %tmp41 = load float*, float** %b
  %tmp42 = load float, float* %tmp41
  %cmp43 = fcmp ult float %tmp40, %tmp42
  %cmp_value44 = zext i1 %cmp43 to i32
  %cmp_ptr = alloca i1
  store i1 false, i1* %cmp_ptr
  %tobool45 = icmp ne i32 %cmp_value38, 0
  br i1 %tobool45, label %"and(br1)", label %"and(end)"

while_end:                                        ; preds = %while_cond
  %ptr75 = load float*, float** %a
  %varval76 = load float, float* %ptr75
  store float 5.500000e+00, float* %ptr75
  %tmp77 = load float, float* %c
  ret float %tmp77

if_yes:                                           ; preds = %"or(end)65"
  %tmp70 = load float*, float** %a
  %tmp71 = load float, float* %tmp70
  ret float %tmp71

if_no:                                            ; preds = %"or(end)65"
  %ptr = load float*, float** %a
  %varval72 = load float, float* %ptr
  store float 8.500000e+00, float* %ptr
  br label %if_end

if_end:                                           ; preds = %if_no
  %ptr73 = load float*, float** %b
  %varval74 = load float, float* %ptr73
  store float 0x401CCCCCC0000000, float* %ptr73
  br label %while_cond

"and(br1)":                                       ; preds = %while_loop
  %tobool46 = icmp ne i32 %cmp_value44, 0
  br i1 %tobool46, label %"and(br2)", label %"and(end)"

"and(br2)":                                       ; preds = %"and(br1)"
  store i1 true, i1* %cmp_ptr
  br label %"and(end)"

"and(end)":                                       ; preds = %"and(br2)", %"and(br1)", %while_loop
  %cmp_value47 = load i1, i1* %cmp_ptr
  %cmp_value48 = zext i1 %cmp_value47 to i32
  %tmp49 = load float, float* %c
  %tmp50 = load float*, float** %a
  %tmp51 = load float, float* %tmp50
  %cmp52 = fcmp ueq float %tmp49, %tmp51
  %cmp_value53 = zext i1 %cmp52 to i32
  %tmp54 = load i32, i32* %x
  %tobool55 = icmp ne i32 %tmp54, 0
  %lnot = xor i1 %tobool55, true
  %lnot.ext = zext i1 %lnot to i32
  %cmp_ptr56 = alloca i1
  store i1 true, i1* %cmp_ptr56
  %tobool57 = icmp ne i32 %cmp_value53, 0
  br i1 %tobool57, label %"or(end)", label %"or(br1)"

"or(br1)":                                        ; preds = %"and(end)"
  %tobool58 = icmp ne i32 %lnot.ext, 0
  br i1 %tobool58, label %"or(end)", label %"or(br2)"

"or(br2)":                                        ; preds = %"or(br1)"
  store i1 false, i1* %cmp_ptr56
  br label %"or(end)"

"or(end)":                                        ; preds = %"or(br2)", %"or(br1)", %"and(end)"
  %cmp_value59 = load i1, i1* %cmp_ptr56
  %cmp_value60 = zext i1 %cmp_value59 to i32
  %cmp_ptr61 = alloca i1
  store i1 true, i1* %cmp_ptr61
  %tobool62 = icmp ne i32 %cmp_value48, 0
  br i1 %tobool62, label %"or(end)65", label %"or(br1)63"

"or(br1)63":                                      ; preds = %"or(end)"
  %tobool66 = icmp ne i32 %cmp_value60, 0
  br i1 %tobool66, label %"or(end)65", label %"or(br2)64"

"or(br2)64":                                      ; preds = %"or(br1)63"
  store i1 false, i1* %cmp_ptr61
  br label %"or(end)65"

"or(end)65":                                      ; preds = %"or(br2)64", %"or(br1)63", %"or(end)"
  %cmp_value67 = load i1, i1* %cmp_ptr61
  %cmp_value68 = zext i1 %cmp_value67 to i32
  %tobool69 = icmp ne i32 %cmp_value68, 0
  br i1 %tobool69, label %if_yes, label %if_no
}

define i32 @run() {
entry:
  %getargf = call float @getargf(i32 0)
  %a = alloca float
  store float %getargf, float* %a
  %getargf1 = call float @getargf(i32 1)
  %b = alloca float
  store float %getargf1, float* %b
  %getargf2 = call float @getargf(i32 2)
  %c = alloca float
  store float %getargf2, float* %c
  %printslit = call i32 (...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @printslit_str, i32 0, i32 0))
  %tmp = load float, float* %a
  %fpext = fpext float %tmp to double
  %print = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float, i32 0, i32 0), double %fpext)
  %tmp3 = load float, float* %b
  %fpext4 = fpext float %tmp3 to double
  %print5 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.1, i32 0, i32 0), double %fpext4)
  %tmp6 = load float, float* %c
  %fpext7 = fpext float %tmp6 to double
  %print8 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.2, i32 0, i32 0), double %fpext7)
  %tmp9 = load float, float* %c
  %test1 = call float @test1(float* %a, float* %b, float %tmp9)
  %printslit10 = call i32 (...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @printslit_str.3, i32 0, i32 0))
  %tmp11 = load float, float* %a
  %fpext12 = fpext float %tmp11 to double
  %print13 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.4, i32 0, i32 0), double %fpext12)
  %tmp14 = load float, float* %b
  %fpext15 = fpext float %tmp14 to double
  %print16 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.5, i32 0, i32 0), double %fpext15)
  %tmp17 = load float, float* %c
  %fpext18 = fpext float %tmp17 to double
  %print19 = call i32 (...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @print_float.6, i32 0, i32 0), double %fpext18)
  ret i32 0
}

; Function Attrs: nounwind
declare void @llvm.stackprotector(i8*, i8**) #0

attributes #0 = { nounwind }
