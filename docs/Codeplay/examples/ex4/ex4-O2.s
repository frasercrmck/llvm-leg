	.text
	.file	"ex4.ll"
	.globl	foo
	.type	foo,@function
foo:                                    # @foo
# BB#0:
	sub sp, sp, #4 ; prologue
	movw r1, #2
	str r1, [sp]
	add r0, r0, #2
	add sp, sp, #4 ; epilogue
	bx lr
.Ltmp0:
	.size	foo, .Ltmp0-foo


	.ident	"clang version 3.5.0 "
