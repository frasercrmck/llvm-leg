	.text
	.file	"ex3.ll"
	.globl	foo
	.type	foo,@function
foo:                                    # @foo
# BB#0:                                 # %entry
	mul r0, r0, r1
	add r0, r0, r2
# BB#1:                                 # %exit
	bx lr
.Ltmp0:
	.size	foo, .Ltmp0-foo


	.ident	"clang version 3.5.0 "
