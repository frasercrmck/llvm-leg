	.text
	.file	"ex7.ll"
	.globl	ex7
	.type	ex7,@function
ex7:                                    # @ex7
# BB#0:                                 # %entry
	ldr r0, [r0]
	add r0, r0, #42
	str r0, [r1]
	bx lr
.Ltmp0:
	.size	ex7, .Ltmp0-ex7


	.ident	"clang version 3.6.0 "
	.section	".note.GNU-stack","",@progbits
