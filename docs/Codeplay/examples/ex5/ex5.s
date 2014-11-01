	.text
	.file	"../../docs/Codeplay/examples/ex5/ex5.ll"
	.globl	foo
	.type	foo,@function
foo:                                    # @foo
# BB#0:
	; Write 0x00010002 to r1.
	movw r1, #2 ; Write 0x00000002
	movt r1, #1 ; Write 0x0001XXXX
	add r0, r0, r1
	bx lr
.Ltmp0:
	.size	foo, .Ltmp0-foo


	.ident	"clang version 3.5.0 "
	.section	".note.GNU-stack","",@progbits
