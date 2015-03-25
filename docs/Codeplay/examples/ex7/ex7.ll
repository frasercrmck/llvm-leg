; ModuleID = 'ex6.ll'
target datalayout = "e-m:e-p:32:32-i1:8:32-i8:8:32-i16:16:32-i64:32-f64:32-a:0:32-n32"
target triple = "leg"

; Function Attrs: nounwind uwtable
define void @ex7(i32* nocapture readonly %src, i32* nocapture %dst) #0 {
entry:
  %0 = load i32* %src, align 4, !tbaa !1
  %add = add nsw i32 %0, 42
  store i32 %add, i32* %dst, align 4, !tbaa !1
  ret void
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.6.0 "}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
