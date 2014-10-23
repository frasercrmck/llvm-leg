	.text
	.file	"ex4.ll"
	.globl	foo
	.type	foo,@function
foo:                                    # @foo
# BB#0:
	sub sp, sp, #8 ; prologue
	mov r1, r0
	movw r2, #2
	str r2, [sp, #4]
	add r0, r0, #2
	str r1, [sp]
	add sp, sp, #8 ; epilogue
	bx lr
.Ltmp0:
	.size	foo, .Ltmp0-foo


	.ident	"clang version 3.5.0 "
