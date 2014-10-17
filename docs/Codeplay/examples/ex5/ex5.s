	.text
	.file	"../../docs/Codeplay/examples/ex5/ex5.ll"
	.globl	foo
	.type	foo,@function
foo:                                    # @foo
# BB#0:
	; We want to write 0x00010002 to the register.
	movw r1, #2 ; Write 0x00000002 (write low bits, clear high bits)
	movt r1, #1 ; Write 0x0001XXXX (write high bits, don't touch low bits)
	add r0, r0, r1
	bx lr
.Ltmp0:
	.size	foo, .Ltmp0-foo


	.ident	"clang version 3.5.0 "
	.section	".note.GNU-stack","",@progbits
