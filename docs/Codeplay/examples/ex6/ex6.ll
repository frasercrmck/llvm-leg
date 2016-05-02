; ModuleID = 'ex6.ll'
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32"
target triple = "leg"

; Function Attrs: nounwind readonly
define i32 @left_shift(i32 %a, i32 %b) #0 {
    %c = shl i32 %a, %b
    %d = shl i32 %c, 10
    ret i32 %d
}

define i32 @right_shift_logical(i32 %a, i32 %b) #0 {
    %c = lshr i32 %a, %b
    %d = lshr i32 %c, 11
    ret i32 %d
}

define i32 @right_shift_arithmetic(i32 %a, i32 %b) #0 {
    %c = ashr i32 %a, %b
    %d = ashr i32 %c, 12
    ret i32 %d
}

define i32 @bit_and(i32 %a, i32 %b) #0 {
    %c = and i32 %a, %b
    %d = and i32 %c, 10
    ret i32 %d
}

define i32 @bit_or(i32 %a, i32 %b) #0 {
    %c = or i32 %a, %b
    %d = or i32 %c, 11
    ret i32 %d
}

define i32 @bit_xor(i32 %a, i32 %b) #0 {
    %c = xor i32 %a, %b
    %d = xor i32 %c, 12
    ret i32 %d
}

attributes #0 = { nounwind readonly "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
