; ModuleID = 'branch0.ll'
source_filename = "branch0.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
entry:
  call void @__DSE_Init__()
  %retval = alloca i32, align 4
  call void @__DSE_Alloca__(i32 0, i32* %retval)
  %x = alloca i32, align 4
  call void @__DSE_Alloca__(i32 1, i32* %x)
  %y = alloca i32, align 4
  call void @__DSE_Alloca__(i32 2, i32* %y)
  %z = alloca i32, align 4
  call void @__DSE_Alloca__(i32 3, i32* %z)
  call void @__DSE_Const__(i32 0)
  call void @__DSE_Store__(i32* %retval)
  store i32 0, i32* %retval, align 4
  call void @__DSE_Input__(i32* %x, i32 0)
  call void @__DSE_Input__(i32* %y, i32 1)
  call void @__DSE_Input__(i32* %z, i32 2)
  call void @__DSE_Load__(i32 4, i32* %x)
  %0 = load i32, i32* %x, align 4
  call void @__DSE_Register__(i32 4)
  call void @__DSE_Const__(i32 0)
  call void @__DSE_ICmp__(i32 5, i32 32)
  %cmp = icmp eq i32 %0, 0
  call void @__DSE_Branch__(i32 0, i32 5, i1 %cmp)
  br i1 %cmp, label %if.then, label %if.end3

if.then:                                          ; preds = %entry
  call void @__DSE_Load__(i32 6, i32* %y)
  %1 = load i32, i32* %y, align 4
  call void @__DSE_Load__(i32 7, i32* %z)
  %2 = load i32, i32* %z, align 4
  call void @__DSE_Register__(i32 6)
  call void @__DSE_Register__(i32 7)
  call void @__DSE_ICmp__(i32 8, i32 32)
  %cmp1 = icmp eq i32 %1, %2
  call void @__DSE_Branch__(i32 1, i32 8, i1 %cmp1)
  br i1 %cmp1, label %if.then2, label %if.end

if.then2:                                         ; preds = %if.then
  call void @__DSE_Load__(i32 9, i32* %x)
  %3 = load i32, i32* %x, align 4
  call void @__DSE_Load__(i32 10, i32* %y)
  %4 = load i32, i32* %y, align 4
  call void @__DSE_Load__(i32 11, i32* %z)
  %5 = load i32, i32* %z, align 4
  call void @__DSE_Register__(i32 10)
  call void @__DSE_Register__(i32 11)
  call void @__DSE_BinOp__(i32 12, i32 15)
  %sub = sub nsw i32 %4, %5
  call void @__DSE_Register__(i32 9)
  call void @__DSE_Register__(i32 12)
  call void @__DSE_BinOp__(i32 13, i32 20)
  %div = sdiv i32 %3, %sub
  call void @__DSE_Register__(i32 13)
  call void @__DSE_Store__(i32* %x)
  store i32 %div, i32* %x, align 4
  br label %if.end

if.end:                                           ; preds = %if.then2, %if.then
  br label %if.end3

if.end3:                                          ; preds = %if.end, %entry
  ret i32 0
}

declare dso_local void @__DSE_Input__(i32*, i32) #1

declare void @__DSE_Init__()

declare void @__DSE_Alloca__(i32, i32*)

declare void @__DSE_Const__(i32)

declare void @__DSE_Store__(i32*)

declare void @__DSE_Load__(i32, i32*)

declare void @__DSE_Register__(i32)

declare void @__DSE_ICmp__(i32, i32)

declare void @__DSE_Branch__(i32, i32, i1)

declare void @__DSE_BinOp__(i32, i32)

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 12.0.1-19ubuntu3"}
